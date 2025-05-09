#pragma once
#include"bmp.h"
#include <stdint.h>
typedef enum {
    COLOR_UNCOLORED = -1,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_YELLOW,
    COLOR_PURPLE,
    COLOR_CYAN,
    COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_PINK,
    COLOR_LIME,
    COLOR_MAX
} Color;

typedef struct Region {
    int id;
    Color color;
    int pixel_count;
    struct Region *next;
} Region;

typedef struct AdjNode {
    int region_id;
    struct AdjNode* next;
}AdjNode;

typedef struct GraphNode {
    int region_id;
    Color color;
    AdjNode* adjNode_list;
}GraphNode;

typedef struct Graph {
    int region_count;
    GraphNode* nodes;
}Graph;

Graph* build_adjacency_graph(int** region_map, int width, int height, int region_count);
void free_graph(Graph* graph);

