#include <stdio.h>
#include <stdlib.h>
#include "routing_analyzer.h"

/**
 * main - Entry point for the Cloud Service Data Routing Analyzer
 *
 * Return: EXIT_SUCCESS on successful completion
 */
int main(void)
{
	Graph network;
	char primary_dc;
	RoutingResult result;

	printf("\nInitializing Cloud Service Data Routing Network...\n");
	graph_build_cloud_network(&network);

	printf("Successfully registered %d Data Centers and %d directed routes.\n\n",
	       network.num_vertices, network.num_edges);

	/* 1. Run Bellman-Ford starting from Primary Data Center 'A' */
	primary_dc = 'A';
	result = run_bellman_ford(&network, primary_dc);

	/* 2. Display the comprehensive Routing Table (Task 5) */
	print_routing_table(&result, &network);

	/* 3. Demonstrate specific queries as requested in Task 2 */
	printf("\n--- Specific Destination Query Example (Task 2) ---\n");
	print_destination_summary(&result, &network, 'G');

	/* Additional destination queries */
	print_destination_summary(&result, &network, 'H');
	print_destination_summary(&result, &network, 'F');

	/* 4. Demonstrate defensive handling of invalid and unreachable data centers */
	printf("--- Robustness & Edge-Case Validation ---\n");
	printf("Testing query for invalid data center 'Z':\n");
	print_destination_summary(&result, &network, 'Z');

	return (EXIT_SUCCESS);
}
