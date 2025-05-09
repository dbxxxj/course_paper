#include <stdlib.h>
#include "graph.h"
#include "bmp.h"

void add_edge(GraphNode* nodes, int from, int to) {// функция для добавления ребра между регионами
	AdjNode* current = nodes[from].adjNode_list;
    while (current) {
        if (current->region_id == to) return;
        current = current->next;
    }
    AdjNode* new_node = malloc(sizeof(AdjNode));
    new_node->region_id = to;
    new_node->next = nodes[from].adjNode_list;
    nodes[from].adjNode_list = new_node;

}
#define RADIUS 2

Graph* build_adjacency_graph(int** region_map, int width, int height, int region_count) {
    Graph* graph = malloc(sizeof(Graph));
    graph->region_count = region_count;
    graph->nodes = calloc(region_count, sizeof(GraphNode));
    for (int i = 0; i < region_count; i++) {
        graph->nodes[i].color = -1; // цвет пока не присвоен
    }
    
    int** added_edges = calloc(region_count, sizeof(int*));
    for (int i = 0; i < region_count; i++) {
        added_edges[i] = calloc(region_count, sizeof(int));
    }

   
    for (int y = 0; y < height; y++) {
        int prev_region = -1; 

        for (int x = 0; x < width; x++) {
            int current = region_map[y][x];

            if (current < 0) continue; 

            
            if (prev_region >= 0 && prev_region != current && !added_edges[prev_region][current]) {
                add_edge(graph->nodes, prev_region, current);
                add_edge(graph->nodes, current, prev_region);
                added_edges[prev_region][current] = 1;
                added_edges[current][prev_region] = 1;
            }

            prev_region = current; 
        }
    }

   
    for (int x = 0; x < width; x++) {
        int prev_region = -1;

        for (int y = 0; y < height; y++) {
            int current = region_map[y][x];

            if (current < 0) continue;

            if (prev_region >= 0 && prev_region != current && !added_edges[prev_region][current]) {
                add_edge(graph->nodes, prev_region, current);
                add_edge(graph->nodes, current, prev_region);
                added_edges[prev_region][current] = 1;
                added_edges[current][prev_region] = 1;
            }

            prev_region = current;
        }
    }

    
    for (int i = 0; i < region_count; i++) {
        free(added_edges[i]);
    }
    free(added_edges);

    return graph;
}

    void free_graph(Graph *graph) {
        if (!graph) return;

        for (int i = 0; i < graph->region_count; i++) {
            AdjNode* current = graph->nodes[i].adjNode_list;
            while (current) {
                AdjNode* tmp = current;
                current = current->next;
                free(tmp);
            }
        }

        free(graph->nodes);
        free(graph);
    }
