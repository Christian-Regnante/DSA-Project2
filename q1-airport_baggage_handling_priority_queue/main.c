#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "baggage_heap.h"

int main(void)
{
	/* Raw priority scores from Question 1 specification */
	const int initial_priorities[] = {56, 23, 91, 34, 72, 48, 85, 17, 63, 79, 42};
	const int n = sizeof(initial_priorities) / sizeof(initial_priorities[0]);
	Container initial_containers[16];
	MaxHeap *heap;
	Container urgent_x;
	Container removed_container;
	int i;

	printf("\n   AUTOMATED AIRPORT BAGGAGE HANDLING SYSTEM - MAX-HEAP PRIORITY QUEUE\n\n");
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
	MaxHeap *order_queue;
	Patient new_patient;
	Patient cleared_patient;
	Patient treated_patient;
	int i;

	printf("\n********************************************************************************\n");
	printf("     HOSPITAL EMERGENCY DEPARTMENT - TRIAGE PRIORITY SYSTEM (MAX-HEAP)\n");
	printf("********************************************************************************\n\n");

	/* Display raw input data */
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

	printf(">>> [TASK 1]: Building array-based Max-Heap from patient cohort...\n");
	printf("    Executing bottom-up heap construction in O(n) time...\n");
	heap_build(triage_queue, initial_patients, total_initial);

	print_heap_array(triage_queue, "TASK 1: Resulting Initial Max-Heap");
	print_heap_tree(triage_queue);

	/* =========================================================================
	 * TASK 2: GENERATE THE TREATMENT ORDER
	 * ========================================================================= */
	order_queue = heap_create(32);
	if (!order_queue)
	{
		heap_destroy(triage_queue);
		return (EXIT_FAILURE);
	}
	heap_build(order_queue, initial_patients, total_initial);

	printf(">>> [TASK 2]: Generating Treatment Order (Repeated Max-Extraction)...\n");
	printf("    Extracting patients in descending order of triage priority score:\n\n");

	while (heap_extract_max(order_queue, &treated_patient))
	{
		printf("Patient %s (%s) — Priority %d\n",
		       treated_patient.id, treated_patient.name, treated_patient.score);
	}
	printf("\n    (Queue Size: %d — All initial patients scheduled in priority order)\n\n",
	       order_queue->size);
	heap_destroy(order_queue);

	/* =========================================================================
	 * TASK 3: NEW EMERGENCY PATIENT (P08: Kofi, Score: 98)
	 * ========================================================================= */
	strncpy(new_patient.id, "P08", sizeof(new_patient.id) - 1);
	strncpy(new_patient.name, "Kofi", sizeof(new_patient.name) - 1);
	new_patient.score = 98;

	printf(">>> [TASK 3]: New Critical Emergency Patient Arrives!\n");
	printf("    Patient ID: %s | Name: %s | Priority Score: %d\n",
	       new_patient.id, new_patient.name, new_patient.score);
	printf("    Inserting into the existing Max-Heap and restoring heap property via sift_up()...\n");
	heap_insert(triage_queue, new_patient);

	print_heap_array(triage_queue, "TASK 3: Max-Heap After Inserting Kofi (P08: 98)");
	print_heap_tree(triage_queue);

	/* =========================================================================
	 * TASK 4: PATIENT CLEARED (Remove P08)
	 * ========================================================================= */
	printf(">>> [TASK 4]: Patient P08 has been treated and cleared from the emergency queue...\n");
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

	/* Clean up resources */
	heap_destroy(triage_queue);

	return (EXIT_SUCCESS);
}

	/* Initialize containers with IDs 'A' through 'K' and descriptive labels */
	for (i = 0; i < n; i++)
	{
		snprintf(initial_containers[i].id, sizeof(initial_containers[i].id), "%c", 'A' + i);
		snprintf(initial_containers[i].name, sizeof(initial_containers[i].name),
		         "Container-%c (Flight-B%02d)", 'A' + i, (i + 1) * 10);
		initial_containers[i].priority = initial_priorities[i];
	}

	printf(">>> [Input Received]: %d baggage containers with priority scores:\n", n);
	for (i = 0; i < n; i++)
		printf("    %s (%s) => Priority: %d\n",
		       initial_containers[i].id, initial_containers[i].name, initial_containers[i].priority);
	printf("\n");

	/* =========================================================================
	 * TASK 1: BUILD THE MAX-HEAP
	 * ========================================================================= */
	heap = heap_create(32);
	if (!heap)
	{
		fprintf(stderr, "Fatal error: Unable to create Max-Heap.\n");
		return (EXIT_FAILURE);
	}

	printf(">>> [TASK 1]: Constructing array-based Max-Heap using bottom-up heapify\n");
	heap_build(heap, initial_containers, n);
	print_heap_array(heap, "TASK 1: Initial Max-Heap Construction Result");
	print_heap_tree(heap);

	/* =========================================================================
	 * TASK 2: URGENT BAGGAGE CONTAINER (X: 100)
	 * ========================================================================= */
	snprintf(urgent_x.id, sizeof(urgent_x.id), "X");
	snprintf(urgent_x.name, sizeof(urgent_x.name), "Urgent-Cargo-X (VIP-Transfer)");
	urgent_x.priority = 100;

	printf(">>> [TASK 2]: Urgent container %s arrives with Priority Score %d!\n",
	       urgent_x.id, urgent_x.priority);
	printf("    Inserting into the heap and triggering sift-up operation\n");
	heap_insert(heap, urgent_x);
	print_heap_array(heap, "TASK 2: Max-Heap After Inserting Urgent Container X (100)");
	print_heap_tree(heap);

	/* =========================================================================
	 * TASK 3: CANCELLED CONTAINER REMOVAL (Container X)
	 * ========================================================================= */
	printf(">>> [TASK 3]: Container X is cancelled and must be removed from the queue\n");
	printf("    Removing Container X and triggering sift-down / re-heapify operation\n");
	if (heap_remove_by_id(heap, "X", &removed_container))
	{
		printf("    Successfully removed: [%s] %s with Priority %d\n\n",
		       removed_container.id, removed_container.name, removed_container.priority);
	}
	else
	{
		fprintf(stderr, "Error: Container X not found in heap!\n");
	}
	print_heap_array(heap, "TASK 3: Max-Heap After Removal of Container X");
	print_heap_tree(heap);

	/* =========================================================================
	 * RUBRIC REQUIREMENT: EXTRACT IN DESCENDING PRIORITY ORDER
	 * ========================================================================= */
	printf(">>> [RUBRIC DEMONSTRATION]: Processing / Extracting all containers in descending priority order:\n");
	printf("--------------------------------------------------------------------------------\n");
	printf(" %-6s | %-4s | %-24s | %-8s\n", "Rank", "ID", "Container Name", "Priority");
	printf("--------------------------------------------------------------------------------\n");
	i = 1;
	while (heap_extract_max(heap, &removed_container))
	{
		printf(" #%-5d | %-4s | %-24s | %-8d\n",
		       i++, removed_container.id, removed_container.name, removed_container.priority);
	}
	printf("--------------------------------------------------------------------------------\n");
	printf(" Heap is now empty (Size: %d). Processing complete.\n\n", heap->size);

	heap_destroy(heap);
	return (EXIT_SUCCESS);
}
