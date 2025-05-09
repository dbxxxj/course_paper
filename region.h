#pragma once
#include "bmp.h"
typedef struct {
    int x;
    int y;
} Point;

int** allocate_region_map(int width, int height);
void free_region_map(int** region_map, int height);
void flood_fill_region_map(RGB** pixels, int width, int height, int** region_map);
int is_border_color(RGB pixel);
