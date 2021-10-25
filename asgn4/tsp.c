#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

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

// DFS Function for finding Hamiltonian paths.

void dfs(Graph *G, uint32_t v, Path *cur, Path *shortest, char *cities[], FILE *outfile) {
    dfs_calls++;
    // Return if current path length exceeds shortest path.
    if (path_length(cur) >= path_length(shortest) && path_length(shortest))
        return;

    // First check if current path visited every city.
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

    // Check which cities current path can go to.
    for (uint32_t i = 0; i < n; i++) {
        if (i == START_VERTEX)
            continue;
        if (graph_has_edge(G, v, i) && !graph_visited(G, i)) {
            graph_mark_visited(G, i);
            path_push_vertex(cur, i, G);

            // Create a temp path for the next DFS call.
            Path *temp = path_create();
            path_copy(temp, cur);
            dfs(G, i, temp, shortest, cities, outfile);
            path_delete(&temp);
            path_pop_vertex(cur, &j, G);
            graph_mark_unvisited(G, i);
        }
    }
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

    // Reading command=line options.
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

    // Reading inputs.
    if (fscanf(infile, "%" SCNu32 "\n", &n) != 1 || n > VERTICES) {
        fprintf(stderr, "Error: malformed number of vertices.\n");
        return 0;
    }
    char *buf = (char *) calloc(1024, sizeof(char));
    char **cities = (char **) calloc(n, sizeof(char *));
    for (uint32_t i = 0; i < n; i++) {
        fgets(buf, 1024, infile);
        for (uint32_t j = 0; j < 1024; j++) {
            if (buf[j] == '\n') {
                buf[j] = '\0';
                break;
            }
        }
        cities[i] = strdup(buf);
    }
    Graph *g = graph_create(n, undirected);
    uint32_t city1 = 0, city2 = 0, weight = 0;
    int reading = fscanf(infile, "%" SCNu32 " %" SCNu32 " %" SCNu32 "\n", &city1, &city2, &weight);
    while (reading == 3) {
        if (!graph_add_edge(g, city1, city2, weight)) {
            fprintf(stderr, "Error: malformed edge.\n");
            return 0;
        }
        if (undirected)
            graph_add_edge(g, city2, city1, weight);
        reading = fscanf(infile, "%" SCNu32 " %" SCNu32 " %" SCNu32 "\n", &city1, &city2, &weight);
    }
    if (reading != EOF) {
        fprintf(stderr, "Error: malformed edge.\n");
    }

    // Starting DFS.
    Path *cur = path_create(), *shortest = path_create();
    path_push_vertex(cur, 0, g);
    graph_mark_unvisited(g, 0);
    path_copy(shortest, cur);
    dfs(g, 0, cur, shortest, cities, outfile);
    path_print(shortest, outfile, cities);
    fprintf(outfile, "Total recursive calls: %d\n", dfs_calls);

    // Time to free memory.

    path_delete(&cur);
    path_delete(&shortest);
    graph_delete(&g);
    fclose(infile);
    fclose(outfile);
    for (uint32_t i = 0; i < n; i++) {
        free(cities[i]);
        cities[i] = NULL;
    }
    free(cities);
    cities = NULL;
    free(buf);
    buf = NULL;
    free(cwd);
    cwd = NULL;
}
