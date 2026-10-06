#ifndef ROUTING_ANALYZER_H
#define ROUTING_ANALYZER_H

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

#define MAX_VERTICES 26
#define MAX_EDGES 100
#define INF INT_MAX

/**
 * struct Edge - Represents a directed data-transfer route
 * @src: Index of the source data center
 * @dest: Index of the destination data center
 * @weight: Net transfer cost (can be negative due to credits)
 */
typedef struct Edge
{
	int src;
	int dest;
	int weight;
} Edge;

/**
 * struct Graph - Weighted directed graph of data center network
 * @num_vertices: Count of distinct registered data centers
 * @num_edges: Count of directed transfer routes
 * @vertex_names: Character identifiers for each vertex
 * @edges: Array of directed weighted routes
 */
typedef struct Graph
{
	int num_vertices;
	int num_edges;
	char vertex_names[MAX_VERTICES];
	Edge edges[MAX_EDGES];
} Graph;

/**
 * struct RoutingResult - Stores shortest-path and cycle analysis results
 * @source_index: Index of the source data center
 * @num_vertices: Total vertices in the analyzed graph
 * @dist: Array of minimum cumulative transfer costs from source
 * @parent: Predecessor array for path reconstruction
 * @has_negative_cycle: Flag indicating presence of a negative cycle
 */
typedef struct RoutingResult
{
	int source_index;
	int num_vertices;
	int dist[MAX_VERTICES];
	int parent[MAX_VERTICES];
	bool has_negative_cycle;
} RoutingResult;

/* Graph Operations */
void graph_init(Graph *g);
int graph_add_vertex(Graph *g, char name);
bool graph_add_edge(Graph *g, char src_name, char dest_name, int weight);
void graph_build_cloud_network(Graph *g);

/* Vertex Mapping & Validation */
int vertex_name_to_index(const Graph *g, char name);
char vertex_index_to_name(const Graph *g, int index);
bool vertex_is_valid(const Graph *g, char name);

/* Bellman-Ford Shortest-Path & Analysis */
RoutingResult run_bellman_ford(const Graph *g, char source_name);
void print_path(const RoutingResult *res, const Graph *g, int dest_index);
void print_routing_table(const RoutingResult *res, const Graph *g);
void print_destination_summary(const RoutingResult *res, const Graph *g,
			       char dest_name);

#endif /* ROUTING_ANALYZER_H */
