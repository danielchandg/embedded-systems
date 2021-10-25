#include "graph.h"

#include "vertices.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Credit to Darrell Long for these functions.

typedef struct Graph {
    uint32_t vertices;
    bool undirected;
    bool visited[VERTICES];
    uint32_t matrix[VERTICES][VERTICES];
} Graph;

Graph *graph_create(uint32_t vertices, bool undirected) {
    Graph *G = (Graph *) malloc(sizeof(Graph));
    G->vertices = vertices;
    G->undirected = undirected;
    for (uint32_t i = 0; i < VERTICES; i++)
        G->visited[i] = false;
    for (uint32_t i = 0; i < VERTICES; i++) {
        for (uint32_t j = 0; j < VERTICES; j++)
            G->matrix[i][j] = 0;
    }
    return G;
}
void graph_delete(Graph **G) {
    free(*G);
    *G = NULL;
    return;
}
uint32_t graph_vertices(Graph *G) {
    return G->vertices;
}
bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k) {
    if (i < 0 || j < 0 || i >= graph_vertices(G) || j >= graph_vertices(G))
        return false;
    G->matrix[i][j] = k;
    if (G->undirected)
        G->matrix[j][i] = k;
    return true;
}
bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {
    return graph_edge_weight(G, i, j) > 0;
}
uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {
    if (i < 0 || j < 0 || i >= graph_vertices(G) || j >= graph_vertices(G))
        return 0;
    return G->matrix[i][j];
}
bool graph_visited(Graph *G, uint32_t v) {
    if (v < 0 || v >= graph_vertices(G))
        return false;
    return G->visited[v];
}
void graph_mark_visited(Graph *G, uint32_t v) {
    if (v >= 0 && v < graph_vertices(G))
        G->visited[v] = true;
}
void graph_mark_unvisited(Graph *G, uint32_t v) {
    if (v >= 0 && v < graph_vertices(G))
        G->visited[v] = false;
}
void graph_print(Graph *G) {
    printf("----- This Graph -----\n");
    printf("visited:\n");
    for (uint32_t i = 0; i < G->vertices; i++)
        printf("%d, ", G->visited[i]);
    printf("\n----------------------\n");
}
