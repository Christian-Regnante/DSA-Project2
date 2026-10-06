#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "routing_analyzer.h"

/**
 * test_baseline_network - Validates all shortest paths and costs from A
 */
static void test_baseline_network(void)
{
	Graph g;
	RoutingResult res;

	printf("[TEST 1] Baseline Network Verification...\n");
	graph_build_cloud_network(&g);

	assert(g.num_vertices == 10);
	assert(g.num_edges == 15);

	res = run_bellman_ford(&g, 'A');

	/* 1. Verify cycle status */
	assert(!res.has_negative_cycle);

	/* 2. Verify all exact distances from source A */
	assert(res.dist[vertex_name_to_index(&g, 'A')] == 0);
	assert(res.dist[vertex_name_to_index(&g, 'B')] == 6);
	assert(res.dist[vertex_name_to_index(&g, 'C')] == 12);
	assert(res.dist[vertex_name_to_index(&g, 'D')] == 12);
	assert(res.dist[vertex_name_to_index(&g, 'J')] == 13);
	assert(res.dist[vertex_name_to_index(&g, 'E')] == 16);
	assert(res.dist[vertex_name_to_index(&g, 'I')] == 14);
	assert(res.dist[vertex_name_to_index(&g, 'F')] == 16);
	assert(res.dist[vertex_name_to_index(&g, 'G')] == 3);
	assert(res.dist[vertex_name_to_index(&g, 'H')] == 16);

	/* 3. Verify predecessors / path linkages */
	assert(res.parent[vertex_name_to_index(&g, 'B')] == vertex_name_to_index(&g, 'A'));
	assert(res.parent[vertex_name_to_index(&g, 'C')] == vertex_name_to_index(&g, 'B'));
	assert(res.parent[vertex_name_to_index(&g, 'D')] == vertex_name_to_index(&g, 'B'));
	assert(res.parent[vertex_name_to_index(&g, 'J')] == vertex_name_to_index(&g, 'B'));
	assert(res.parent[vertex_name_to_index(&g, 'E')] == vertex_name_to_index(&g, 'J'));
	assert(res.parent[vertex_name_to_index(&g, 'I')] == vertex_name_to_index(&g, 'E'));
	assert(res.parent[vertex_name_to_index(&g, 'F')] == vertex_name_to_index(&g, 'I'));
	assert(res.parent[vertex_name_to_index(&g, 'G')] == vertex_name_to_index(&g, 'C'));
	assert(res.parent[vertex_name_to_index(&g, 'H')] == vertex_name_to_index(&g, 'G'));

	printf("  -> PASS: All 10 data centers have optimal costs and parent paths.\n\n");
}

/**
 * test_negative_weight_handling - Confirms negative edge benefits are captured
 */
static void test_negative_weight_handling(void)
{
	Graph g;
	RoutingResult res;
	int c_idx, g_idx, e_idx, i_idx;

	printf("[TEST 2] Negative-Weight Handling (Task 3)...\n");
	graph_build_cloud_network(&g);
	res = run_bellman_ford(&g, 'A');

	c_idx = vertex_name_to_index(&g, 'C');
	g_idx = vertex_name_to_index(&g, 'G');
	e_idx = vertex_name_to_index(&g, 'E');
	i_idx = vertex_name_to_index(&g, 'I');

	/* C -> G has weight -9: cost to G (3) must be less than cost to C (12) */
	assert(res.dist[g_idx] < res.dist[c_idx]);
	assert(res.dist[g_idx] == res.dist[c_idx] - 9);

	/* E -> I has weight -2: cost to I (14) must be less than cost to E (16) */
	assert(res.dist[i_idx] < res.dist[e_idx]);
	assert(res.dist[i_idx] == res.dist[e_idx] - 2);

	printf("  -> PASS: Negative edge weights successfully reduce cumulative routing cost.\n\n");
}

/**
 * test_negative_cycle_detection - Verifies detection when a cycle is injected
 */
static void test_negative_cycle_detection(void)
{
	Graph g;
	RoutingResult res_clean, res_cycle;

	printf("[TEST 3] Negative-Cycle Detection (Task 4)...\n");

	/* Case A: Clean network has no negative cycle */
	graph_build_cloud_network(&g);
	res_clean = run_bellman_ford(&g, 'A');
	assert(res_clean.has_negative_cycle == false);
	printf("  -> Verified: Clean network reports NO negative cycle.\n");

	/* Case B: Inject negative cycle between F and I */
	/* Originally F->I = 2, I->F = 2 (sum +4). Change I->F to -5 -> cycle sum -3 */
	graph_init(&g);
	graph_add_edge(&g, 'A', 'B', 1);
	graph_add_edge(&g, 'B', 'F', 2);
	graph_add_edge(&g, 'F', 'I', 1);
	graph_add_edge(&g, 'I', 'F', -5); /* Negative cycle: F -> I -> F = -4 */

	res_cycle = run_bellman_ford(&g, 'A');
	assert(res_cycle.has_negative_cycle == true);
	printf("  -> Verified: Network with negative cycle properly detects cycle.\n\n");
}

/**
 * test_unreachable_node - Verifies behavior for an unreachable data center
 */
static void test_unreachable_node(void)
{
	Graph g;
	RoutingResult res;
	int u_idx;

	printf("[TEST 4] Unreachable Node Detection (Constraint)...\n");
	graph_build_cloud_network(&g);

	/* Add isolated node 'K' with no incoming edges */
	graph_add_vertex(&g, 'K');
	u_idx = vertex_name_to_index(&g, 'K');
	assert(u_idx != -1);

	res = run_bellman_ford(&g, 'A');
	assert(res.dist[u_idx] == INF);
	assert(res.parent[u_idx] == -1);

	printf("  -> PASS: Unreachable node correctly assigned INF and parent -1.\n\n");
}

/**
 * test_invalid_queries - Verifies robustness against invalid queries
 */
static void test_invalid_queries(void)
{
	Graph g;
	RoutingResult res;

	printf("[TEST 5] Invalid Data Center Name Handling (Constraint)...\n");
	graph_build_cloud_network(&g);

	/* Querying invalid source should return empty result without crash */
	res = run_bellman_ford(&g, 'Z');
	assert(res.source_index == -1);

	/* Querying invalid vertex index */
	assert(vertex_name_to_index(&g, 'Z') == -1);
	assert(vertex_index_to_name(&g, 99) == '?');
	assert(!vertex_is_valid(&g, 'Z'));

	printf("  -> PASS: Program safely rejects invalid data center queries without crashing.\n\n");
}

int main(void)
{
	printf("===============================================================\n");
	printf("         RUNNING COMPREHENSIVE REQUIREMENTS TEST SUITE         \n");
	printf("===============================================================\n\n");

	test_baseline_network();
	test_negative_weight_handling();
	test_negative_cycle_detection();
	test_unreachable_node();
	test_invalid_queries();

	printf("===============================================================\n");
	printf("   ALL 5 REQUIREMENT TESTS PASSED PERFECTLY WITH ZERO ERRORS   \n");
	printf("===============================================================\n");

	return (EXIT_SUCCESS);
}
