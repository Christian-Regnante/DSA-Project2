#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "routing_analyzer.h"

/**
 * graph_init - Initializes an empty graph structure
 * @g: Pointer to graph
 */
void graph_init(Graph *g)
{
	if (!g)
		return;

	g->num_vertices = 0;
	g->num_edges = 0;
	memset(g->vertex_names, 0, sizeof(g->vertex_names));
}

/**
 * vertex_name_to_index - Finds the index of a vertex given its character name
 * @g: Pointer to graph
 * @name: Character label of vertex
 *
 * Return: Zero-based index if found, -1 if not found
 */
int vertex_name_to_index(const Graph *g, char name)
{
	int i;

	if (!g)
		return (-1);

	for (i = 0; i < g->num_vertices; i++)
	{
		if (g->vertex_names[i] == name)
			return (i);
	}
	return (-1);
}

/**
 * vertex_index_to_name - Retrieves character name for a given index
 * @g: Pointer to graph
 * @index: Vertex index
 *
 * Return: Character name if valid index, '?' if out of bounds
 */
char vertex_index_to_name(const Graph *g, int index)
{
	if (!g || index < 0 || index >= g->num_vertices)
		return ('?');

	return (g->vertex_names[index]);
}

/**
 * vertex_is_valid - Checks if a vertex exists in the graph
 * @g: Pointer to graph
 * @name: Character name to query
 *
 * Return: true if vertex exists, false otherwise
 */
bool vertex_is_valid(const Graph *g, char name)
{
	return (vertex_name_to_index(g, name) != -1);
}

/**
 * graph_add_vertex - Registers a vertex if it is not already in the graph
 * @g: Pointer to graph
 * @name: Character name of the vertex
 *
 * Return: Index of the vertex, or -1 if capacity exceeded
 */
int graph_add_vertex(Graph *g, char name)
{
	int existing_idx;
	int new_idx;

	if (!g)
		return (-1);

	existing_idx = vertex_name_to_index(g, name);
	if (existing_idx != -1)
		return (existing_idx);

	if (g->num_vertices >= MAX_VERTICES)
	{
		fprintf(stderr, "Error: Max vertex capacity reached.\n");
		return (-1);
	}

	new_idx = g->num_vertices++;
	g->vertex_names[new_idx] = name;
	return (new_idx);
}

/**
 * graph_add_edge - Inserts a directed weighted edge between two vertices
 * @g: Pointer to graph
 * @src_name: Source data center character
 * @dest_name: Destination data center character
 * @weight: Net transfer cost (positive or negative)
 *
 * Return: true on success, false if max edges exceeded
 */
bool graph_add_edge(Graph *g, char src_name, char dest_name, int weight)
{
	int u, v;

	if (!g)
		return (false);

	if (g->num_edges >= MAX_EDGES)
	{
		fprintf(stderr, "Error: Max edge capacity reached.\n");
		return (false);
	}

	u = graph_add_vertex(g, src_name);
	v = graph_add_vertex(g, dest_name);

	if (u == -1 || v == -1)
		return (false);

	g->edges[g->num_edges].src = u;
	g->edges[g->num_edges].dest = v;
	g->edges[g->num_edges].weight = weight;
	g->num_edges++;

	return (true);
}

/**
 * graph_build_cloud_network - Populates graph with the exact network
 * @g: Pointer to graph
 */
void graph_build_cloud_network(Graph *g)
{
	graph_init(g);

	/* 15 directed routes defined in Question 5 */
	graph_add_edge(g, 'A', 'B',  6);
	graph_add_edge(g, 'A', 'D', 16);
	graph_add_edge(g, 'B', 'C',  6);
	graph_add_edge(g, 'B', 'D',  6);
	graph_add_edge(g, 'B', 'J',  7);
	graph_add_edge(g, 'C', 'G', -9);
	graph_add_edge(g, 'D', 'E',  7);
	graph_add_edge(g, 'D', 'J',  8);
	graph_add_edge(g, 'E', 'F', 10);
	graph_add_edge(g, 'E', 'I', -2);
	graph_add_edge(g, 'F', 'G',  4);
	graph_add_edge(g, 'F', 'I',  2);
	graph_add_edge(g, 'G', 'H', 13);
	graph_add_edge(g, 'I', 'F',  2);
	graph_add_edge(g, 'J', 'E',  3);
}

/**
 * relax_edges - Relaxes all edges in the graph once
 * @g: Pointer to graph
 * @res: Pointer to routing result
 *
 * Return: true if any distance was updated, false otherwise
 */
static bool relax_edges(const Graph *g, RoutingResult *res)
{
	bool updated = false;
	int j, u, v, weight;

	for (j = 0; j < g->num_edges; j++)
	{
		u = g->edges[j].src;
		v = g->edges[j].dest;
		weight = g->edges[j].weight;

		if (res->dist[u] != INF && res->dist[u] + weight < res->dist[v])
		{
			res->dist[v] = res->dist[u] + weight;
			res->parent[v] = u;
			updated = true;
		}
	}
	return (updated);
}

/**
 * has_negative_cycle - Tests if any edge can still be relaxed
 * @g: Pointer to graph
 * @res: Pointer to routing result
 *
 * Return: true if a reachable negative cycle exists, false otherwise
 */
static bool has_negative_cycle(const Graph *g, const RoutingResult *res)
{
	int j, u, v, weight;

	for (j = 0; j < g->num_edges; j++)
	{
		u = g->edges[j].src;
		v = g->edges[j].dest;
		weight = g->edges[j].weight;

		if (res->dist[u] != INF && res->dist[u] + weight < res->dist[v])
			return (true);
	}
	return (false);
}

/**
 * run_bellman_ford - Runs the Bellman-Ford shortest-path algorithm
 * @g: Pointer to graph
 * @source_name: Starting data center
 *
 * Return: RoutingResult structure containing distances and parents
 */
RoutingResult run_bellman_ford(const Graph *g, char source_name)
{
	RoutingResult res;
	int i, src_idx;

	res.source_index = -1;
	res.num_vertices = 0;
	res.has_negative_cycle = false;

	if (!g)
		return (res);

	res.num_vertices = g->num_vertices;
	for (i = 0; i < MAX_VERTICES; i++)
	{
		res.dist[i] = INF;
		res.parent[i] = -1;
	}

	src_idx = vertex_name_to_index(g, source_name);
	if (src_idx == -1)
	{
		fprintf(stderr, "Error: DC '%c' not in network.\n", source_name);
		return (res);
	}

	res.source_index = src_idx;
	res.dist[src_idx] = 0;

	for (i = 1; i <= g->num_vertices - 1; i++)
	{
		if (!relax_edges(g, &res))
			break;
	}

	res.has_negative_cycle = has_negative_cycle(g, &res);
	return (res);
}

/**
 * print_path_recursive - Helper to trace parent pointers recursively
 * @parent: Array of parent pointers
 * @g: Pointer to graph
 * @curr: Current node index
 */
static void print_path_recursive(const int parent[], const Graph *g, int curr)
{
	if (curr == -1)
		return;

	if (parent[curr] != -1)
	{
		print_path_recursive(parent, g, parent[curr]);
		printf(" → ");
	}
	printf("%c", g->vertex_names[curr]);
}

/**
 * print_path - Public path printer for a destination index
 * @res: RoutingResult structure
 * @g: Pointer to graph
 * @dest_index: Destination node index
 */
void print_path(const RoutingResult *res, const Graph *g, int dest_index)
{
	if (!res || !g || dest_index < 0 || dest_index >= g->num_vertices)
	{
		printf("None");
		return;
	}

	if (dest_index == res->source_index)
	{
		printf("%c", g->vertex_names[dest_index]);
		return;
	}

	if (res->dist[dest_index] == INF || res->parent[dest_index] == -1)
	{
		printf("None (Unreachable)");
		return;
	}

	print_path_recursive(res->parent, g, dest_index);
}

/**
 * print_routing_table - Displays the full routing table
 * @res: Pointer to routing result
 * @g: Pointer to graph
 */
void print_routing_table(const RoutingResult *res, const Graph *g)
{
	int i;

	if (!res || !g || res->source_index == -1)
	{
		fprintf(stderr, "Cannot print routing table: Invalid input.\n");
		return;
	}

	printf("======================================================================\n");
	printf("              CLOUD SERVICE DATA ROUTING ANALYZER                    \n");
	printf("======================================================================\n");

	if (res->has_negative_cycle)
	{
		printf("Status: Negative-weight cycle detected.\n");
		printf("Shortest-path results may be undefined.\n");
		printf("======================================================================\n");
		return;
	}

	printf("Status: No negative-weight cycle detected.\n");
	printf("Source: %c\n\n", g->vertex_names[res->source_index]);
	printf("%-15s %-15s %-35s\n", "Destination", "Shortest Cost", "Path");
	printf("----------------------------------------------------------------------\n");

	for (i = 0; i < g->num_vertices; i++)
	{
		if (i == res->source_index)
			continue;

		printf("%-15c ", g->vertex_names[i]);
		if (res->dist[i] == INF)
		{
			printf("%-15s %-35s\n", "Unreachable", "None");
		}
		else
		{
			printf("%-15d ", res->dist[i]);
			print_path(res, g, i);
			printf("\n");
		}
	}
	printf("======================================================================\n");
}

/**
 * print_destination_summary - Displays single destination query format
 * @res: Pointer to routing result
 * @g: Pointer to graph
 * @dest_name: Destination data center character
 */
void print_destination_summary(const RoutingResult *res, const Graph *g,
			       char dest_name)
{
	int idx;

	if (!res || !g)
		return;

	idx = vertex_name_to_index(g, dest_name);
	if (idx == -1)
	{
		printf("Destination: %c (Invalid data center)\n", dest_name);
		printf("Path: None\n");
		printf("Cost: N/A\n\n");
		return;
	}

	printf("Destination: %c\n", dest_name);
	if (res->has_negative_cycle)
	{
		printf("Path: Undefined (Negative cycle detected)\n");
		printf("Cost: Undefined\n\n");
		return;
	}

	if (res->dist[idx] == INF)
	{
		printf("Path: None (Unreachable from source)\n");
		printf("Cost: ∞\n\n");
	}
	else
	{
		printf("Path: ");
		print_path(res, g, idx);
		printf("\n");
		printf("Cost: %d\n\n", res->dist[idx]);
	}
}
