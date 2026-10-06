#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "gateway_network.h"

/* ========================================================================= */
/* Queue Operations (Module 3, Unit 6 - FIFO Behavior)                       */
/* ========================================================================= */

void queue_init(Queue *q)
{
    q->front = 0;
    q->rear = 0;
    q->count = 0;
}

bool queue_is_empty(const Queue *q)
{
    return q->count == 0;
}

bool queue_is_full(const Queue *q)
{
    return q->count == NUM_GATEWAYS;
}

void queue_enqueue(Queue *q, int value)
{
    if (queue_is_full(q))
    {
        fprintf(stderr, "Queue overflow error\n");
        return;
    }
    q->items[q->rear] = value;
    q->rear = (q->rear + 1) % NUM_GATEWAYS;
    q->count++;
}

int queue_dequeue(Queue *q)
{
    if (queue_is_empty(q))
    {
        fprintf(stderr, "Queue underflow error\n");
        return -1;
    }
    int val = q->items[q->front];
    q->front = (q->front + 1) % NUM_GATEWAYS;
    q->count--;
    return val;
}

/* ========================================================================= */
/* Name & Index Conversions                                                  */
/* ========================================================================= */

int gateway_name_to_index(char name)
{
    char upper = (char)toupper((unsigned char)name);
    if (upper >= 'A' && upper < 'A' + NUM_GATEWAYS)
    {
        return upper - 'A';
    }
    return -1;
}

char gateway_index_to_name(int index)
{
    if (index >= 0 && index < NUM_GATEWAYS)
    {
        return (char)('A' + index);
    }
    return '?';
}

bool gateway_is_valid(char name)
{
    return gateway_name_to_index(name) != -1;
}

/* ========================================================================= */
/* Graph Construction                                                        */
/* ========================================================================= */

void graph_init(Graph *g)
{
    g->num_vertices = NUM_GATEWAYS;
    for (int i = 0; i < NUM_GATEWAYS; i++)
    {
        for (int j = 0; j < NUM_GATEWAYS; j++)
        {
            g->adj[i][j] = 0;
        }
    }
}

void graph_add_edge(Graph *g, char u_name, char v_name, int weight)
{
    int u = gateway_name_to_index(u_name);
    int v = gateway_name_to_index(v_name);

    if (u != -1 && v != -1 && weight > 0)
    {
        /* Undirected network: symmetric latency */
        g->adj[u][v] = weight;
        g->adj[v][u] = weight;
    }
}

void graph_build_iot_network(Graph *g)
{
    graph_init(g);

    /* Edges defined by IoT Gateway Network specification */
    graph_add_edge(g, 'A', 'B', 6);
    graph_add_edge(g, 'A', 'D', 12);
    graph_add_edge(g, 'B', 'D', 5);
    graph_add_edge(g, 'B', 'C', 11);
    graph_add_edge(g, 'C', 'D', 17);
    graph_add_edge(g, 'C', 'G', 25);
    graph_add_edge(g, 'D', 'E', 22);
    graph_add_edge(g, 'D', 'F', 15);
    graph_add_edge(g, 'E', 'F', 10);
    graph_add_edge(g, 'F', 'G', 22);
}

/* ========================================================================= */
/* BFS Connectivity Analysis & Communication-Link Evaluation                */
/* ========================================================================= */

AnalysisResult analyze_gateway_connectivity(const Graph *g, int start_index)
{
    AnalysisResult res;
    res.start_vertex = start_index;
    res.bfs_count = 0;
    res.direct_count = 0;
    res.slowest_gateway = -1;
    res.max_transfer_time = -1;

    bool visited[NUM_GATEWAYS] = {false};
    Queue q;
    queue_init(&q);

    /* Mark the starting gateway as visited and insert into queue */
    visited[start_index] = true;
    queue_enqueue(&q, start_index);

    while (!queue_is_empty(&q))
    {
        int curr = queue_dequeue(&q);
        res.bfs_order[res.bfs_count++] = curr;

        /* Scan all neighbors of current gateway in alphabetical order */
        for (int neighbor = 0; neighbor < NUM_GATEWAYS; neighbor++)
        {
            if (g->adj[curr][neighbor] > 0 && !visited[neighbor])
            {
                /* Crucial BFS invariant: mark visited at discovery */
                visited[neighbor] = true;
                queue_enqueue(&q, neighbor);

                /* If current node is the start node, this is a 1-hop neighbor */
                if (curr == start_index)
                {
                    res.direct_neighbors[res.direct_count] = neighbor;
                    res.direct_weights[res.direct_count] = g->adj[start_index][neighbor];
                    res.direct_count++;
                }
            }
        }
    }

    /* Communication Analysis: find the directly connected gateway with highest transfer time */
    for (int i = 0; i < res.direct_count; i++)
    {
        if (res.direct_weights[i] > res.max_transfer_time)
        {
            res.max_transfer_time = res.direct_weights[i];
            res.slowest_gateway = res.direct_neighbors[i];
        }
    }

    return res;
}

void print_analysis_report(const AnalysisResult *res)
{
    printf("\n========================================================\n");
    printf("     IoT GATEWAY CONNECTIVITY ANALYSIS REPORT           \n");
    printf("========================================================\n");
    printf("Starting Gateway: %c\n\n", gateway_index_to_name(res->start_vertex));

    /* 1. BFS Traversal Order */
    printf("--- Task 2: BFS Connectivity Analysis ---\n");
    printf("BFS Traversal Order: ");
    for (int i = 0; i < res->bfs_count; i++)
    {
        printf("%c%s", gateway_index_to_name(res->bfs_order[i]),
               (i < res->bfs_count - 1) ? " -> " : "");
    }
    printf("\n");

    /* 2. Directly Connected Gateways (1-hop) */
    printf("Directly Connected Gateways (1-hop): ");
    if (res->direct_count == 0)
    {
        printf("None (Isolated Gateway)\n");
    }
    else
    {
        for (int i = 0; i < res->direct_count; i++)
        {
            printf("%c ", gateway_index_to_name(res->direct_neighbors[i]));
        }
        printf("\n\n");

        /* 3. Communication Link Analysis */
        printf("--- Task 3: Communication Analysis ---\n");
        printf("+-----------------------+--------------------------+\n");
        printf("| Communication Link    | Data-Transfer Time (ms)  |\n");
        printf("+-----------------------+--------------------------+\n");
        for (int i = 0; i < res->direct_count; i++)
        {
            printf("| Gateway %c <-> %c       | %16d ms     |\n",
                   gateway_index_to_name(res->start_vertex),
                   gateway_index_to_name(res->direct_neighbors[i]),
                   res->direct_weights[i]);
        }
        printf("+-----------------------+--------------------------+\n");

        printf("\nDirectly connected gateway with HIGHEST transfer time:\n");
        printf("  >> Gateway %c (Transfer Time: %d ms)\n",
               gateway_index_to_name(res->slowest_gateway),
               res->max_transfer_time);
    }
    printf("========================================================\n\n");
}
