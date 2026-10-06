#ifndef EV_NETWORK_H
#define EV_NETWORK_H

#include <stddef.h>
#include <stdbool.h>

#define NUM_STATIONS 7
#define NUM_EDGES 10

/**
 * station_index_to_name - Converts numeric station index to character name
 * @index: 0-based station index (0 -> 'A', 6 -> 'G')
 * Return: Station character name or '?' if invalid
 */
char station_index_to_name(int index);

/**
 * station_name_to_index - Converts station character name to numeric index
 * @name: Station name ('A' - 'G' or 'a' - 'g')
 * Return: 0-based index or -1 if invalid
 */
int station_name_to_index(char name);

/**
 * struct Edge - Represents an undirected weighted connection
 * @u: First endpoint station index
 * @v: Second endpoint station index
 * @weight: Installation cost in thousands of dollars
 */
typedef struct {
    int u;
    int v;
    int weight;
} Edge;

/**
 * struct EVGraph - Models the EV charging-station power network
 * @num_vertices: Total number of charging stations (7)
 * @num_edges: Total number of available cable connections (10)
 * @adj_matrix: Symmetric adjacency matrix representation
 * @edges: Array of all available cable connections
 */
typedef struct {
    int num_vertices;
    int num_edges;
    int adj_matrix[NUM_STATIONS][NUM_STATIONS];
    Edge edges[NUM_EDGES];
} EVGraph;

/**
 * struct DSU - Disjoint Set Union structure for cycle detection
 * @parent: Parent pointers for each station set
 * @rank: Rank/tree depth for union-by-rank optimization
 */
typedef struct {
    int parent[NUM_STATIONS];
    int rank[NUM_STATIONS];
} DSU;

/**
 * struct MSTResult - Stores the outcome of Kruskal's algorithm
 * @selected_edges: Array of edges forming the Minimum Spanning Tree (V - 1)
 * @edge_count: Number of edges selected
 * @total_cost: Sum of installation costs in thousands of dollars
 */
typedef struct {
    Edge selected_edges[NUM_STATIONS - 1];
    int edge_count;
    int total_cost;
} MSTResult;

/* Graph initialization */
void ev_graph_init(EVGraph *graph);

/* Task 1: Display Adjacency Matrix */
void ev_graph_print_adjacency_matrix(const EVGraph *graph);

/* DSU operations */
void dsu_init(DSU *dsu, int n);
int dsu_find(DSU *dsu, int i);
bool dsu_union(DSU *dsu, int i, int j);

/* Task 2: Apply Kruskal's Algorithm with detailed trace */
MSTResult ev_graph_kruskal_mst(const EVGraph *graph, bool verbose);

/* Task 3: Display Selected Connections */
void ev_graph_print_selected_connections(const MSTResult *result);

/* Task 4: Display Total Installation Cost */
void ev_graph_print_total_cost(const MSTResult *result);

/* Combined report execution */
void run_ev_network_analysis(void);

#endif /* EV_NETWORK_H */
