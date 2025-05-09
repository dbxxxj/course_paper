#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "region.h"
#include "graph.h"
#include "colorr.h"
#include <locale.h>
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    if (argc != 2) {
        fprintf(stderr, "�������������: %s <������� BMP-����>\n", argv[0]);
        return 1;
    }

    const char* input_filename = argv[1];
    FILE* fp = fopen(input_filename, "rb");
    if (!fp) {
        perror("�� ������� ������� ������� ����");
        return 1;
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    if (!read_bmp_headers(fp, &fileHeader, &infoHeader)) {
        fclose(fp);
        return 1;
    }
    

    RGB** pixels = read_bmp_pixels(fp, &fileHeader, &infoHeader);
    fclose(fp);

    if (!pixels) {
        fprintf(stderr, "������ ��� ������ �������� BMP\n");
        return 1;
    }

    int width = infoHeader.biWidth;
    int height = abs(infoHeader.biHeight);
    int** region_map = allocate_region_map(width, height);

    flood_fill_region_map(pixels, width, height, region_map);
    
    int region_count = 0;
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            if (region_map[y][x] + 1 > region_count)
                region_count = region_map[y][x] + 1;

    Graph* graph = build_adjacency_graph(region_map, width, height, region_count);
    color_regions(graph);

    // ������������� ������� �� ����� �������
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int region_id = region_map[y][x];
            if (region_id >= 0 && region_id < region_count) {
                Color color_index = graph->nodes[region_id].color;
                pixels[y][x] = get_color_by_index(color_index);
            }
        }
    }

    if (!write_bmp("output.bmp", &fileHeader, &infoHeader, pixels)) {
        fprintf(stderr, "�� ������� ��������� ���� output.bmp\n");
        free_graph(graph);
        free_region_map(region_map, height);
        free_pixels(pixels, height);
        return 1;
    }

    printf("����������� ������� ��������� ��� output.bmp\n");

    free_graph(graph);
    free_region_map(region_map, height);
    free_pixels(pixels, height);

    return 0;
}
