#include "region.h"
#include <stdlib.h>

int is_border_color(RGB pixel) {
    return pixel.r < 50 && pixel.g < 50 && pixel.b < 50;
}
int** allocate_region_map(int width, int height) {
    int** map = malloc(sizeof(int*) * height);
    for (int i = 0; i < height; i++) {
        map[i] = malloc(sizeof(int) * width);
        for (int j = 0; j < width; j++) {
            map[i][j] = -1;
        }
    }
    return map;
}

void free_region_map(int** region_map, int height) {
    for (int i = 0; i < height; i++) {
        free(region_map[i]);
    }
    free(region_map);
}
int in_bounds(int x, int y, int width, int height) {
    return x >= 0 && x < width && y >= 0 && y < height;
}
void flood_fill(RGB** pixels, int** map, int width, int height, int start_x, int start_y, int region_id) {
    Point* stack = malloc(width * height * sizeof(Point));
    if (!stack) return;  

    int stack_size = 0;
    stack[stack_size++] = (Point){ start_x, start_y };

    while (stack_size > 0) {
        Point p = stack[--stack_size];

        if (!in_bounds(p.x, p.y, width, height)) continue;
        if (map[p.y][p.x] != -1) continue;
        if (is_border_color(pixels[p.y][p.x])) continue;

        map[p.y][p.x] = region_id;

        // Добавляем соседей в стек
        if (in_bounds(p.x + 1, p.y, width, height)) stack[stack_size++] = (Point){ p.x + 1, p.y };
        if (in_bounds(p.x - 1, p.y, width, height)) stack[stack_size++] = (Point){ p.x - 1, p.y };
        if (in_bounds(p.x, p.y + 1, width, height)) stack[stack_size++] = (Point){ p.x, p.y + 1 };
        if (in_bounds(p.x, p.y - 1, width, height)) stack[stack_size++] = (Point){ p.x, p.y - 1 };
    }

    free(stack);
}

void flood_fill_region_map(RGB** pixels, int width, int height, int** region_map) {
    int current_region = 0;

   
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            region_map[y][x] = -1;  
        }
    }

    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            
            if (region_map[y][x] != -1 || is_border_color(pixels[y][x])) {
                continue;
            }

            Point* queue = malloc(width * height * sizeof(Point));
            int queue_start = 0;
            int queue_end = 0;

            queue[queue_end++] = (Point){ x, y };
            region_map[y][x] = current_region;

            while (queue_start < queue_end) {
                Point p = queue[queue_start++];

                
                int dx[4] = { 1, -1, 0, 0 };
                int dy[4] = { 0, 0, 1, -1 };

                for (int i = 0; i < 4; i++) {
                    int nx = p.x + dx[i];
                    int ny = p.y + dy[i];

                    if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                        region_map[ny][nx] == -1 &&
                        !is_border_color(pixels[ny][nx])) {

                        region_map[ny][nx] = current_region;
                        queue[queue_end++] = (Point){ nx, ny };
                    }
                }
            }

            free(queue);
            current_region++;
        }
    }

    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (region_map[y][x] == -1 && !is_border_color(pixels[y][x])) {
                printf("Предупреждение: пиксель (%d,%d) не был залит!\n", x, y);
            }
        }
    }
}

        

