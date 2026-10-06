#ifndef GATEWAY_NETWORK_H
#define GATEWAY_NETWORK_H

#include <stdbool.h>
#include <stddef.h>

#define NUM_GATEWAYS 7

/**
 * struct Queue - Circular/linear queue for BFS traversal
 * @items: Array storing gateway node indices
 * @front: Front index
 * @rear: Rear index
 * @count: Number of elements currently in the queue
 */
typedef struct {
    int items[NUM_GATEWAYS];
    int front;
    int rear;
    int count;
} Queue;

/**
 * struct Graph - Represents the IoT Gateway network
 * @adj: Adjacency matrix holding data-transfer times (ms), 0 if no direct link
 * @num_vertices: Total number of gateways
 */
typedef struct {
    int adj[NUM_GATEWAYS][NUM_GATEWAYS];
    int num_vertices;
} Graph;

/**
 * struct AnalysisResult - Stores results of the BFS connectivity analysis
 * @start_vertex: Index of the starting gateway
 * @bfs_order: Ordered list of gateways visited during BFS
 * @bfs_count: Total gateways reached by BFS
 * @direct_neighbors: 1-hop gateways directly connected to start
 * @direct_weights: Transfer times (ms) to each 1-hop gateway
 * @direct_count: Number of 1-hop gateways
 * @slowest_gateway: 1-hop gateway with the highest transfer time
 * @max_transfer_time: The highest transfer time (ms)
 */
typedef struct {
    int start_vertex;
    int bfs_order[NUM_GATEWAYS];
    int bfs_count;
    int direct_neighbors[NUM_GATEWAYS];
    int direct_weights[NUM_GATEWAYS];
    int direct_count;
    int slowest_gateway;
    int max_transfer_time;
} AnalysisResult;

/* Queue Operations */
void queue_init(Queue *q);
bool queue_is_empty(const Queue *q);
bool queue_is_full(const Queue *q);
void queue_enqueue(Queue *q, int value);
int queue_dequeue(Queue *q);

/* Graph Operations */
void graph_init(Graph *g);
void graph_add_edge(Graph *g, char u_name, char v_name, int weight);
void graph_build_iot_network(Graph *g);

/* Helper & Conversion Functions */
int gateway_name_to_index(char name);
char gateway_index_to_name(int index);
bool gateway_is_valid(char name);

/* BFS & Connectivity Analysis */
AnalysisResult analyze_gateway_connectivity(const Graph *g, int start_index);
void print_analysis_report(const AnalysisResult *res);

#endif /* GATEWAY_NETWORK_H */
