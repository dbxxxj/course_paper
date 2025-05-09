#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bmp.h"

void up_progress(long current, long total) {
    static int last_percent = -1;
    int percent = (int)((100.0 * current) / total);

    if (percent != last_percent) {
        last_percent = percent;
        printf("\rПрогресс: %d%%", percent);
        fflush(stdout);
    }
}

int read_bmp_headers(FILE* fp, BMPFileHeader* fileHeader, BMPInfoHeader* infoHeader) {
    if (!fp) return 0;

    fread(fileHeader, sizeof(BMPFileHeader), 1, fp);
    if (fileHeader->bfType != 0x4D42) {
        fprintf(stderr, "Неверный формат файла (не BMP)\n");
        return 0;
    }

    fread(infoHeader, sizeof(BMPInfoHeader), 1, fp);
    if (infoHeader->biBitCount != 24 || infoHeader->biCompression != 0) {
        fprintf(stderr, "Поддерживаются только 24-битные несжатые BMP\n");
        return 0;
    }
    return 1;
}

RGB** read_bmp_pixels(FILE* fp, const BMPFileHeader* fileHeader, const BMPInfoHeader* infoHeader) {
    int width = infoHeader->biWidth;
    int height = abs(infoHeader->biHeight);
    int padding = (4 - (width * 3) % 4) % 4;

    RGB** pixels = malloc(height * sizeof(RGB*));
    if (!pixels) return NULL;

    fseek(fp, fileHeader->bfOffbits, SEEK_SET);

    long progress = 0;
    long total = width * height;


    for (int i = height - 1; i >= 0; i--) {
        pixels[i] = malloc(width * sizeof(RGB));
        if (!pixels[i]) return NULL;

        for (int j = 0; j < width; j++) {
            uint8_t bgr[3];
            fread(bgr, 1, 3, fp);
            pixels[i][j].b = bgr[0];
            pixels[i][j].g = bgr[1];
            pixels[i][j].r = bgr[2];

            progress++;
            up_progress(progress, total);
        }

        fseek(fp, padding, SEEK_CUR); // пропуск паддинга
    }

    printf("\rФайл успешно открыт\n");
    return pixels;
}

int write_bmp(const char* filename, const BMPFileHeader* fileHeader, const BMPInfoHeader* infoHeader, RGB** pixels) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;

    fwrite(fileHeader, sizeof(BMPFileHeader), 1, fp);
    fwrite(infoHeader, sizeof(BMPInfoHeader), 1, fp);

    int width = infoHeader->biWidth;
    int height = abs(infoHeader->biHeight);
    int padding = (4 - (width * 3) % 4) % 4;
    uint8_t pad[3] = { 0, 0, 0 };

    long progress = 0;
    long total = width * height;

    for (int i = height - 1; i >= 0; i--) {
        fwrite(pixels[i], sizeof(RGB), width, fp);
        fwrite(pad, 1, padding, fp); // записываем паддинг
        for (int j = 0; j < width; j++) {
            progress++;
            up_progress(progress, total);
        }
    }

    fclose(fp);
    printf("\n");
    return 1;
}
void free_pixels(RGB** pixels, int height) {
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}


