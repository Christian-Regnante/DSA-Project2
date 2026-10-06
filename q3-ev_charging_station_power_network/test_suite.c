#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "ev_network.h"

/* Helper BFS to verify MST connectivity and acyclic nature */
static bool verify_mst_connectivity_and_acyclic(const MSTResult *mst)
{
    /* Build adjacency list from MST edges */
    int adj[NUM_STATIONS][NUM_STATIONS] = {0};
    for (int i = 0; i < mst->edge_count; i++)
    {
        int u = mst->selected_edges[i].u;
        int v = mst->selected_edges[i].v;
        adj[u][v] = 1;
        adj[v][u] = 1;
    }

    /* BFS from node 0 (Station A) */
    bool visited[NUM_STATIONS] = {false};
    int queue[NUM_STATIONS];
    int front = 0, rear = 0;

    visited[0] = true;
    queue[rear++] = 0;
    int visited_count = 1;

    while (front < rear)
    {
        int curr = queue[front++];
        for (int next = 0; next < NUM_STATIONS; next++)
        {
            if (adj[curr][next] && !visited[next])
            {
                visited[next] = true;
                queue[rear++] = next;
                visited_count++;
            }
        }
    }

    /* If all stations are reachable and edge_count == V - 1, it is guaranteed a valid tree */
    return (visited_count == NUM_STATIONS && mst->edge_count == NUM_STATIONS - 1);
}

static void test_task1_adjacency_matrix(const EVGraph *graph)
{
    printf("[TEST] Verifying Task 1: Adjacency Matrix Representation...\n");

    /* 1. Verify dimensions: 7 stations */
    assert(graph->num_vertices == 7);
    assert(graph->num_edges == 10);

    /* 2. Verify all 10 connections */
    assert(graph->adj_matrix[0][1] == 6);   /* A — B */
    assert(graph->adj_matrix[0][3] == 12);  /* A — D */
    assert(graph->adj_matrix[1][3] == 5);   /* B — D */
    assert(graph->adj_matrix[1][2] == 11);  /* B — C */
    assert(graph->adj_matrix[2][3] == 17);  /* C — D */
    assert(graph->adj_matrix[2][6] == 25);  /* C — G */
    assert(graph->adj_matrix[3][4] == 22);  /* D — E */
    assert(graph->adj_matrix[3][5] == 15);  /* D — F */
    assert(graph->adj_matrix[4][5] == 10);  /* E — F */
    assert(graph->adj_matrix[5][6] == 22);  /* F — G */

    /* 3. Verify symmetry for undirected graph */
    for (int i = 0; i < NUM_STATIONS; i++)
    {
        for (int j = 0; j < NUM_STATIONS; j++)
        {
            assert(graph->adj_matrix[i][j] == graph->adj_matrix[j][i]);
        }
    }

    /* 4. Verify no self-loops */
    for (int i = 0; i < NUM_STATIONS; i++)
    {
        assert(graph->adj_matrix[i][i] == 0);
    }

    /* 5. Verify disconnected pairs are 0 */
    assert(graph->adj_matrix[0][2] == 0);   /* A — C */
    assert(graph->adj_matrix[0][4] == 0);   /* A — E */
    assert(graph->adj_matrix[0][5] == 0);   /* A — F */
    assert(graph->adj_matrix[0][6] == 0);   /* A — G */

    printf("  -> PASS: All matrix entries, symmetry, and zero-values verified.\n");
}

static void test_task2_kruskal_algorithm(const EVGraph *graph)
{
    printf("[TEST] Verifying Task 2: Kruskal's Algorithm Execution...\n");

    MSTResult mst = ev_graph_kruskal_mst(graph, false);

    /* 1. Exactly V - 1 = 6 edges */
    assert(mst.edge_count == 6);

    /* 2. Selected edges must be in increasing order of consideration */
    assert(mst.selected_edges[0].weight == 5);   /* B — D */
    assert(mst.selected_edges[1].weight == 6);   /* A — B */
    assert(mst.selected_edges[2].weight == 10);  /* E — F */
    assert(mst.selected_edges[3].weight == 11);  /* B — C */
    assert(mst.selected_edges[4].weight == 15);  /* D — F */
    assert(mst.selected_edges[5].weight == 22);  /* F — G */

    printf("  -> PASS: Kruskal correctly selected 6 edges in greedy order.\n");
}

static void test_task3_mst_properties(const EVGraph *graph)
{
    printf("[TEST] Verifying Task 3: MST Properties (Connectivity, Acyclic, Minimal)...\n");

    MSTResult mst = ev_graph_kruskal_mst(graph, false);

    /* Verify connected and acyclic */
    bool valid_tree = verify_mst_connectivity_and_acyclic(&mst);
    assert(valid_tree == true);

    /* Verify specific connections */
    bool found_bd = false, found_ab = false, found_ef = false;
    bool found_bc = false, found_df = false, found_fg = false;

    for (int i = 0; i < mst.edge_count; i++)
    {
        Edge e = mst.selected_edges[i];
        char u = station_index_to_name(e.u);
        char v = station_index_to_name(e.v);

        if ((u == 'B' && v == 'D') || (u == 'D' && v == 'B')) found_bd = true;
        if ((u == 'A' && v == 'B') || (u == 'B' && v == 'A')) found_ab = true;
        if ((u == 'E' && v == 'F') || (u == 'F' && v == 'E')) found_ef = true;
        if ((u == 'B' && v == 'C') || (u == 'C' && v == 'B')) found_bc = true;
        if ((u == 'D' && v == 'F') || (u == 'F' && v == 'D')) found_df = true;
        if ((u == 'F' && v == 'G') || (u == 'G' && v == 'F')) found_fg = true;
    }

    assert(found_bd && found_ab && found_ef && found_bc && found_df && found_fg);
    printf("  -> PASS: All 6 required connections present; forms a valid spanning tree.\n");
}

static void test_task4_total_cost(const EVGraph *graph)
{
    printf("[TEST] Verifying Task 4: Total Installation Cost Calculation...\n");

    MSTResult mst = ev_graph_kruskal_mst(graph, false);

    /* 5 + 6 + 10 + 11 + 15 + 22 = 69 */
    int expected_cost = 5 + 6 + 10 + 11 + 15 + 22;
    assert(mst.total_cost == 69);
    assert(mst.total_cost == expected_cost);

    printf("  -> PASS: Total cost correctly calculated as %d thousand dollars.\n", mst.total_cost);
}

int main(void)
{
    printf("========================================================\n");
    printf("       EV POWER NETWORK OPTIMIZER - AUTOMATED TESTS      \n");
    printf("========================================================\n");

    EVGraph graph;
    ev_graph_init(&graph);

    test_task1_adjacency_matrix(&graph);
    test_task2_kruskal_algorithm(&graph);
    test_task3_mst_properties(&graph);
    test_task4_total_cost(&graph);

    printf("========================================================\n");
    printf("  ALL 4 TASKS PASSED ALL UNIT TESTS SUCCESSFULLY!       \n");
    printf("========================================================\n");

    return 0;
}
