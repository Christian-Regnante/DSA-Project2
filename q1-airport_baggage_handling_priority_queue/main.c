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

	printf("\n********************************************************************************\n");
	printf("   AUTOMATED AIRPORT BAGGAGE HANDLING SYSTEM - MAX-HEAP PRIORITY QUEUE\n");
	printf("********************************************************************************\n\n");

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

	printf(">>> [TASK 1]: Constructing array-based Max-Heap using bottom-up heapify...\n");
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
	printf("    Inserting into the heap and triggering sift-up operation...\n");
	heap_insert(heap, urgent_x);
	print_heap_array(heap, "TASK 2: Max-Heap After Inserting Urgent Container X (100)");
	print_heap_tree(heap);

	/* =========================================================================
	 * TASK 3: CANCELLED CONTAINER REMOVAL (Container X)
	 * ========================================================================= */
	printf(">>> [TASK 3]: Container X is cancelled and must be removed from the queue...\n");
	printf("    Removing Container X and triggering sift-down / re-heapify operation...\n");
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
