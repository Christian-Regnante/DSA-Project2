#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "triage_heap.h"

/**
 * heap_create - Allocates and initializes an empty MaxHeap
 * @capacity: Initial maximum number of patients the heap can store
 *
 * Return: Pointer to allocated MaxHeap, or NULL on failure
 */
MaxHeap *heap_create(int capacity)
{
	MaxHeap *heap;

	if (capacity <= 0)
		return (NULL);

	heap = (MaxHeap *)malloc(sizeof(MaxHeap));
	if (!heap)
	{
		fprintf(stderr, "Error: Memory allocation failed for MaxHeap struct.\n");
		return (NULL);
	}

	heap->patients = (Patient *)malloc(capacity * sizeof(Patient));
	if (!heap->patients)
	{
		fprintf(stderr, "Error: Memory allocation failed for heap patients array.\n");
		free(heap);
		return (NULL);
	}

	heap->size = 0;
	heap->capacity = capacity;
	return (heap);
}

/**
 * heap_destroy - Deallocates all dynamic memory associated with the heap
 * @heap: Pointer to the MaxHeap
 */
void heap_destroy(MaxHeap *heap)
{
	if (heap)
	{
		if (heap->patients)
			free(heap->patients);
		free(heap);
	}
}

/**
 * swap_patients - Swaps two Patient records in memory
 * @a: Pointer to first Patient
 * @b: Pointer to second Patient
 */
void swap_patients(Patient *a, Patient *b)
{
	Patient temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * sift_up - Restores the Max-Heap property from a given index upwards
 * @heap: Pointer to the MaxHeap
 * @index: Position of the element to bubble up
 */
void sift_up(MaxHeap *heap, int index)
{
	int parent_idx;

	while (index > 0)
	{
		parent_idx = (index - 1) / 2;

		/* If child has higher priority than parent, swap them */
		if (heap->patients[index].score > heap->patients[parent_idx].score)
		{
			swap_patients(&heap->patients[index], &heap->patients[parent_idx]);
			index = parent_idx;
		}
		else
		{
			break;
		}
	}
}

/**
 * sift_down - Restores the Max-Heap property from a given index downwards
 * @heap: Pointer to the MaxHeap
 * @index: Position of the element to sift down
 */
void sift_down(MaxHeap *heap, int index)
{
	int largest = index;
	int left;
	int right;

	while (index < heap->size)
	{
		largest = index;
		left = 2 * index + 1;
		right = 2 * index + 2;

		if (left < heap->size && heap->patients[left].score > heap->patients[largest].score)
			largest = left;

		if (right < heap->size && heap->patients[right].score > heap->patients[largest].score)
			largest = right;

		if (largest != index)
		{
			swap_patients(&heap->patients[index], &heap->patients[largest]);
			index = largest;
		}
		else
		{
			break;
		}
	}
}

/**
 * heap_build - Constructs a Max-Heap from an arbitrary array in O(n) time
 * @heap: Target MaxHeap
 * @records: Array of input patient records
 * @count: Number of records
 */
void heap_build(MaxHeap *heap, const Patient records[], int count)
{
	int i;

	if (!heap || !records || count <= 0)
		return;

	if (count > heap->capacity)
		count = heap->capacity;

	for (i = 0; i < count; i++)
		heap->patients[i] = records[i];

	heap->size = count;

	/* Bottom-up heapify: begin at the last non-leaf node */
	for (i = (heap->size / 2) - 1; i >= 0; i--)
		sift_down(heap, i);
}

/**
 * heap_insert - Inserts a new patient record into the Max-Heap
 * @heap: Pointer to the MaxHeap
 * @patient: Patient record to insert
 *
 * Return: 1 on success, 0 on capacity overflow or failure
 */
int heap_insert(MaxHeap *heap, Patient patient)
{
	if (!heap || heap->size >= heap->capacity)
	{
		fprintf(stderr, "Error: Cannot insert patient, queue capacity reached.\n");
		return (0);
	}

	heap->patients[heap->size] = patient;
	sift_up(heap, heap->size);
	heap->size++;

	return (1);
}

/**
 * heap_extract_max - Removes and returns the highest priority patient from the root
 * @heap: Pointer to the MaxHeap
 * @extracted: Pointer to Patient struct where the extracted data is stored
 *
 * Return: 1 on success, 0 if the heap is empty
 */
int heap_extract_max(MaxHeap *heap, Patient *extracted)
{
	if (!heap || heap->size <= 0)
		return (0);

	if (extracted)
		*extracted = heap->patients[0];

	heap->patients[0] = heap->patients[heap->size - 1];
	heap->size--;

	if (heap->size > 0)
		sift_down(heap, 0);

	return (1);
}

/**
 * heap_remove_by_id - Finds and removes a patient by ID from anywhere in the heap
 * @heap: Pointer to the MaxHeap
 * @id: Patient ID string to search for
 * @removed: Pointer where removed record is copied
 *
 * Return: 1 if found and removed, 0 otherwise
 */
int heap_remove_by_id(MaxHeap *heap, const char *id, Patient *removed)
{
	int target_idx = -1;
	int i;

	if (!heap || !id || heap->size <= 0)
		return (0);

	for (i = 0; i < heap->size; i++)
	{
		if (strcmp(heap->patients[i].id, id) == 0)
		{
			target_idx = i;
			break;
		}
	}

	if (target_idx == -1)
		return (0);

	if (removed)
		*removed = heap->patients[target_idx];

	/* Replace target with last element */
	heap->patients[target_idx] = heap->patients[heap->size - 1];
	heap->size--;

	/* Re-heapify: element may need to move up or down */
	if (target_idx < heap->size)
	{
		sift_down(heap, target_idx);
		sift_up(heap, target_idx);
	}

	return (1);
}

/**
 * is_valid_max_heap - Validates that the Max-Heap invariant holds for all nodes
 * @heap: Pointer to the MaxHeap
 *
 * Return: 1 if valid, 0 if invariant violation detected
 */
int is_valid_max_heap(const MaxHeap *heap)
{
	int i;
	int left;
	int right;

	if (!heap)
		return (0);

	for (i = 0; i <= (heap->size - 1) / 2; i++)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;

		if (left < heap->size && heap->patients[left].score > heap->patients[i].score)
			return (0);

		if (right < heap->size && heap->patients[right].score > heap->patients[i].score)
			return (0);
	}

	return (1);
}

/**
 * print_heap_array - Displays formatted tabular representation of the heap array
 * @heap: Pointer to the MaxHeap
 * @title: Section banner header
 */
void print_heap_array(const MaxHeap *heap, const char *title)
{
	int i;

	printf("================================================================================\n");
	printf(" %s\n", title);
	printf(" (Queue Size: %d, Max-Heap Invariant: %s)\n",
	       heap->size, is_valid_max_heap(heap) ? "SATISFIED [OK]" : "VIOLATED [FAIL]");
	printf("--------------------------------------------------------------------------------\n");
	printf(" %-6s | %-6s | %-16s | %-8s | %-20s\n",
	       "Index", "ID", "Patient Name", "Priority", "Child Nodes");
	printf("--------------------------------------------------------------------------------\n");

	for (i = 0; i < heap->size; i++)
	{
		int l = 2 * i + 1;
		int r = 2 * i + 2;
		char children_info[64] = "None (Leaf)";

		if (l < heap->size && r < heap->size)
			snprintf(children_info, sizeof(children_info), "L:%s(%d), R:%s(%d)",
			         heap->patients[l].id, heap->patients[l].score,
			         heap->patients[r].id, heap->patients[r].score);
		else if (l < heap->size)
			snprintf(children_info, sizeof(children_info), "L:%s(%d)",
			         heap->patients[l].id, heap->patients[l].score);

		printf(" [%2d]   | %-6s | %-16s | %-8d | %s\n",
		       i, heap->patients[i].id, heap->patients[i].name,
		       heap->patients[i].score, children_info);
	}
	printf("================================================================================\n\n");
}

/**
 * print_heap_tree - Visualizes the binary tree structure level-by-level
 * @heap: Pointer to the MaxHeap
 */
void print_heap_tree(const MaxHeap *heap)
{
	int level = 0;
	int level_capacity = 1;
	int count = 0;
	int i;

	if (!heap || heap->size == 0)
	{
		printf("[Empty Tree]\n\n");
		return;
	}

	printf("--- Binary Tree Representation (Level-by-Level) ---\n");
	while (count < heap->size)
	{
		printf(" Level %d: ", level);
		for (i = 0; i < level_capacity && count < heap->size; i++, count++)
		{
			printf("[%s: %s (%d)] ",
			       heap->patients[count].id,
			       heap->patients[count].name,
			       heap->patients[count].score);
		}
		printf("\n");
		level++;
		level_capacity *= 2;
	}
	printf("---------------------------------------------------\n\n");
}
