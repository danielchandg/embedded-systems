#include "path.h"

#include "graph.h"
#include "stack.h"
#include "vertices.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Path {
    Stack *vertices;
    uint32_t length;
} Path;
Path *path_create(void) {
    Path *p = (Path *) malloc(sizeof(Path));
    p->vertices = stack_create(VERTICES + 1);
    p->length = 0;
    return p;
}
void path_delete(Path **p) {
    stack_delete(&((*p)->vertices));
    free(*p);
    *p = NULL;
    return;
}
// Returns false if trying to push path without edge
// to last visited city, or if stack is full.
bool path_push_vertex(Path *p, uint32_t v, Graph *G) {
    if (stack_full(p->vertices))
        return false;
    uint32_t top = -1;
    if (stack_peek(p->vertices, &top)) {
        p->length += graph_edge_weight(G, top, v);
    }

    stack_push(p->vertices, v);
    graph_mark_visited(G, v);
    return true;
}
// Returns false if trying to pop empty path.
// If false, then no Hamiltonian path found.
bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {
    if (!stack_pop(p->vertices, v))
        return false;
    graph_mark_unvisited(G, *v);
    uint32_t top = -1;
    if (stack_peek(p->vertices, &top)) {
        p->length -= graph_edge_weight(G, top, *v);
        return true;
    }
    return true;
}
uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}
uint32_t path_length(Path *p) {
    return p->length;
}
void path_copy(Path *dst, Path *src) {
    //path_delete(&dst);
    //dst = path_create();
    stack_copy(dst->vertices, src->vertices);
    dst->length = src->length;
}
void path_print(Path *p, FILE *outfile, char *cities[]) {
    fprintf(outfile, "Path length: %d\nPath: ", p->length);
    stack_print(p->vertices, outfile, cities);
}
void path_peek(Path *p, uint32_t *top) {
    if (stack_peek(p->vertices, top))
        return;
    fprintf(stdout, "Error in path_peek");
    return;
}
