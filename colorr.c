#include "colorr.h"
#include <stdlib.h>
RGB get_color_by_index(int index) {
    RGB palette[COLOR_MAX] = {
        {102, 102, 255},     // красный
        {102, 255, 102},     // зелёный
        {255, 178, 102},     // синий
        {102, 255, 255},   // жёлтый
        {255, 153, 204},    // фиолетовый
        {255, 255, 103},   // голубой
        {51, 153, 255},   // оранжевый
        {153, 76, 0},   // коричневый
        {153, 51, 255},   // розовый
        {178, 255, 102},   // лазурныq
       
    };
    return palette[index % COLOR_MAX];
}
void color_regions(Graph* graph) {
    int* used = malloc(sizeof(int) * COLOR_MAX);
    for (int i = 0;i < graph->region_count;i++) {
        for (int c = 0; c < COLOR_MAX; c++) {
            used[c] = 0;
        }
        AdjNode* adj = graph->nodes[i].adjNode_list;
        while (adj) {
            int neighbor_id = adj->region_id;
            Color neighbor_color = graph->nodes[neighbor_id].color;

            if (neighbor_color >= 0 && neighbor_color < COLOR_MAX) {
                used[neighbor_color] = 1;
            }
            adj = adj->next;
        }
        for (int c = 0; c < COLOR_MAX; c++) {
            if (!used[c]) {
                graph->nodes[i].color = c;
                break;
            }
        }
    }
    free(used);

}   
