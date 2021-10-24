#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "hvui:o:"

bool verbose = false, undirected = false;
uint32_t n, j;
static uint32_t dfs_calls = 0;
void path_peek(Path *p, uint32_t *top);
void dfs(Graph *G, uint32_t v, Path *cur, Path *shortest, char *cities[], FILE *outfile) {
    dfs_calls++;
    if (path_length(cur) >= path_length(shortest) && path_length(shortest)) {
        return;
    }
    //fprintf(stdout, "Current path vertices & length: %d, %d\n", path_vertices(cur), path_length(cur));
    if (path_vertices(cur) == n) {
        if (graph_has_edge(G, v, 0)) {
            path_push_vertex(cur, 0, G);
            if (verbose)
                path_print(cur, outfile, cities);
            if (path_length(cur) < path_length(shortest) || !path_length(shortest))
                path_copy(shortest, cur);
        }
        return;
    }
    for (uint32_t i = 0; i < n; i++) {
        if (i == START_VERTEX)
            continue;
        //fprintf(stdout, "Path (%d, %d): %d\n", path_vertices(cur), path_length(cur), i);
        if (graph_has_edge(G, v, i) && !graph_visited(G, i)) {
            //if(path_length(cur) >= path_length(shortest) && path_vertices(shortest) > 1){
            //path_print(shortest, outfile, cities);
            //	path_pop_vertex(cur, &j, G);
            //	continue;
            //}
            graph_mark_visited(G, i);
            path_push_vertex(cur, i, G);
            Path *temp = path_create();
            path_copy(temp, cur);
            dfs(G, i, temp, shortest, cities, outfile);
            path_delete(&temp);
            path_pop_vertex(cur, &j, G);
            graph_mark_unvisited(G, i);
        }
    }
    //path_delete(&temp);
    //path_delete(&cur);
}

// Prints out the help string given the current directory.
void help_string(char *cwd, FILE *outfile) {
    fprintf(outfile,
        "SYNOPSIS\n  Traveling Salesman Problem using DFS.\n\nUSAGE\n  %s/tsp [-u] [-v] [-h] [-i "
        "infile] [-o outfile]\n\nOPTIONS  -u             Use undirected graph.\n  -v             "
        "Enable verbose printing.\n  -h             Program usage and help.\n  -i infile      "
        "Input containing graph (default: stdin)\n  -o outfile     Output of computed path "
        "(default: stdout)\n",
        cwd);
}
int main(int argc, char **argv) {
    int64_t opt = 0;
    verbose = false, undirected = false;
    FILE *infile = stdin, *outfile = stdout;
    char *cwd = (char *) calloc(FILENAME_MAX, sizeof(char));
    getcwd(cwd, FILENAME_MAX);

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help_string(cwd, outfile); return 0;
        case 'v': verbose = true; break;
        case 'u': undirected = true; break;
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case '?': help_string(cwd, outfile); return 0;
        }
    }
    if (fscanf(infile, "%" SCNu32 "\n", &n) != 1 || n > VERTICES) {
        fprintf(stderr, "Error: malformed number of vertices.\n");
        return 0;
    }
    //fprintf(stdout, "Number of vertices: %d\n", n);
    char *buf = (char *) calloc(1024, sizeof(char));
    char **cities = (char **) calloc(n, sizeof(char *));
    for (uint32_t i = 0; i < n; i++) {
        //cities[i] = (char *) calloc(1024, sizeof(char));
        fgets(buf, 1024, infile);
        for (uint32_t j = 0; j < 1024; j++) {
            if (buf[j] == '\n') {
                buf[j] = '\0';
                break;
            }
        }
        cities[i] = strdup(buf);
        /*if (fscanf(infile, "%s\n", cities[i]) != 1) {
            fprintf(stderr, "Error: malformed city.\n");
            return 0;
        }*/
        //fprintf(stdout, "City %d: %s\n", i, cities[i]);
    }
    free(buf);
    buf = NULL;
    Graph *g = graph_create(n, undirected);
    uint32_t city1 = 0, city2 = 0, weight = 0;
    int reading = fscanf(infile, "%" SCNu32 " %" SCNu32 " %" SCNu32 "\n", &city1, &city2, &weight);
    while (reading == 3) {
        if (!graph_add_edge(g, city1, city2, weight)) {
            fprintf(stderr, "Error: malformed edge.\n");
            return 0;
        }
        //fprintf(stdout, "Edge from %d to %d with weight %d\n", city1, city2, weight);
        if (undirected)
            graph_add_edge(g, city2, city1, weight);
        reading = fscanf(infile, "%" SCNu32 " %" SCNu32 " %" SCNu32 "\n", &city1, &city2, &weight);
    }
    if (reading != EOF) {
        fprintf(stderr, "Error: malformed edge.\n");
    }
    Path *cur = path_create(), *shortest = path_create();
    path_push_vertex(cur, 0, g);
    graph_mark_unvisited(g, 0);
    path_copy(shortest, cur);
    //fprintf(stdout, "Starting DFS\n");
    dfs(g, 0, cur, shortest, cities, outfile);
    //fprintf(stdout, "DFS complete.\n");
    path_delete(&cur);
    path_print(shortest, outfile, cities);
    fprintf(outfile, "Total recursive calls: %d\n", dfs_calls);
    path_delete(&shortest);
}
