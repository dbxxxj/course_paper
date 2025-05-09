#pragma once
#include <stdint.h>
#include<stdio.h>
#pragma pack(push, 1)

typedef struct {
	uint16_t bfType; // сигнатура файла, т.е. должно быть это:0x4D42
	uint32_t bfSize; // размер всего файла;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffbits;// смещение от начала файла до начала пикселей
}BMPFileHeader;
typedef struct {
    uint32_t biSize;             // Размер этой структуры (обычно 40 байт)
    int32_t  biWidth;            // Ширина изображения в пикселях
    int32_t  biHeight;           // Высота изображения в пикселях (может быть отрицательной для "сверху вниз")
    uint16_t biPlanes;           // Количество цветовых плоскостей (всегда должно быть 1)
    uint16_t biBitCount;         // Количество бит на пиксель (например, 24 для RGB)
    uint32_t biCompression;      // Тип сжатия (0 = BI_RGB, то есть без сжатия)
    uint32_t biSizeImage;        // Размер изображения в байтах (может быть 0 для несжатого)
    int32_t  biXPelsPerMeter;    // Горизонтальное разрешение (пикселей на метр)
    int32_t  biYPelsPerMeter;    // Вертикальное разрешение (пикселей на метр)
    uint32_t biClrUsed;          // Количество используемых цветов (0 означает, что используется максимальное количество)
    uint32_t biClrImportant;     // Количество "важных" цветов (0 означает, что все важны)
} BMPInfoHeader;
#pragma pack(pop)
typedef struct {
	uint8_t r, g, b;
} RGB;


int read_bmp_headers(FILE* fp, BMPFileHeader* fileHeader, BMPInfoHeader* infoHeader);
RGB** read_bmp_pixels(FILE* fp, const BMPFileHeader* fileHeader, const BMPInfoHeader* infoHeader);
int write_bmp(const char* filename, const BMPFileHeader* fileHeader, const BMPInfoHeader* infoHeader, RGB** pixels);
void free_pixels(RGB** pixels, int height);