#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "triage_heap.h"

int main(void)
{
	/* Initial patient cohort from Question 2 specification */
	const Patient initial_patients[] = {
		{"P01", "Amina",  72},
		{"P02", "Daniel", 45},
		{"P03", "Eric",   91},
		{"P04", "Grace",  63},
		{"P05", "Hassan", 88},
		{"P06", "Irene",  54},
		{"P07", "Jean",   76}
	};
	const int total_initial = sizeof(initial_patients) / sizeof(initial_patients[0]);
	MaxHeap *triage_queue;
	Patient new_patient;
	Patient cleared_patient;
	Patient treated_patient;
	int rank;
	int i;

	printf("\n********************************************************************************\n");
	printf("     HOSPITAL EMERGENCY DEPARTMENT - TRIAGE PRIORITY SYSTEM (MAX-HEAP)\n");
	printf("********************************************************************************\n\n");

	/* Display incoming patients */
	printf(">>> [Input Received]: %d waiting patients for assessment:\n", total_initial);
	for (i = 0; i < total_initial; i++)
	{
		printf("    [%s] %-8s => Priority Score: %d\n",
		       initial_patients[i].id,
		       initial_patients[i].name,
		       initial_patients[i].score);
	}
	printf("\n");

	/* =========================================================================
	 * TASK 1: BUILD THE MAX-HEAP
	 * ========================================================================= */
	triage_queue = heap_create(32);
	if (!triage_queue)
	{
		fprintf(stderr, "Fatal error: Unable to allocate triage queue.\n");
		return (EXIT_FAILURE);
	}

	printf(">>> [TASK 1]: Converting patient cohort into an array-based Max-Heap...\n");
	printf("    Executing bottom-up heap construction in O(n) time...\n");
	heap_build(triage_queue, initial_patients, total_initial);

	print_heap_array(triage_queue, "TASK 1: Initial Max-Heap Construction Result");
	print_heap_tree(triage_queue);

	/* =========================================================================
	 * TASK 3: NEW EMERGENCY PATIENT ARRIVAL (P08: Kofi, Score: 98)
	 * ========================================================================= */
	strncpy(new_patient.id, "P08", sizeof(new_patient.id) - 1);
	strncpy(new_patient.name, "Kofi", sizeof(new_patient.name) - 1);
	new_patient.score = 98;

	printf(">>> [TASK 3]: New Critical Emergency Patient Arrives!\n");
	printf("    Patient ID: %s | Name: %s | Priority Score: %d\n",
	       new_patient.id, new_patient.name, new_patient.score);
	printf("    Appending to leaf position and restoring Max-Heap property via sift_up()...\n");
	heap_insert(triage_queue, new_patient);

	print_heap_array(triage_queue, "TASK 3: Max-Heap After Inserting Kofi (P08: 98)");
	print_heap_tree(triage_queue);

	/* =========================================================================
	 * TASK 4: PATIENT CLEARED (Remove P08)
	 * ========================================================================= */
	printf(">>> [TASK 4]: Patient P08 has been assessed and cleared from emergency queue...\n");
	printf("    Removing P08 and restoring Max-Heap property via sift_down()...\n");
	if (heap_remove_by_id(triage_queue, "P08", &cleared_patient))
	{
		printf("    Successfully cleared: [%s] %s (Priority: %d)\n\n",
		       cleared_patient.id, cleared_patient.name, cleared_patient.score);
	}
	else
	{
		fprintf(stderr, "Error: Patient P08 not found in queue!\n");
	}

	print_heap_array(triage_queue, "TASK 4: Max-Heap After Clearing P08");
	print_heap_tree(triage_queue);

	/* =========================================================================
	 * TASK 2: GENERATE THE TREATMENT ORDER
	 * ========================================================================= */
	printf(">>> [TASK 2]: Generating Treatment Order (Repeated Root Extraction)...\n");
	printf("--------------------------------------------------------------------------------\n");
	printf(" %-6s | %-6s | %-16s | %-10s\n", "Order", "ID", "Patient Name", "Priority");
	printf("--------------------------------------------------------------------------------\n");

	rank = 1;
	while (heap_extract_max(triage_queue, &treated_patient))
	{
		printf(" #%-5d | %-6s | %-16s | %-10d\n",
		       rank++, treated_patient.id, treated_patient.name, treated_patient.score);
	}
	printf("--------------------------------------------------------------------------------\n");
	printf(" All patients attended. Emergency queue is now empty (Size: %d).\n\n",
	       triage_queue->size);

	/* Clean up resources */
	heap_destroy(triage_queue);

	return (EXIT_SUCCESS);
}
