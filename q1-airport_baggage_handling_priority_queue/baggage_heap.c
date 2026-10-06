#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "baggage_heap.h"

/**
 * heap_create - Allocates and initializes an empty MaxHeap
 * @capacity: Initial maximum number of elements the heap can store
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
		fprintf(stderr, "Error: Failed to allocate memory for MaxHeap structure.\n");
		return (NULL);
	}

	heap->elements = (Container *)malloc(capacity * sizeof(Container));
	if (!heap->elements)
	{
		fprintf(stderr, "Error: Failed to allocate memory for heap elements array.\n");
		free(heap);
		return (NULL);
	}

	heap->size = 0;
	heap->capacity = capacity;
	return (heap);
}

/**
 * heap_destroy - Deallocates all resources associated with the heap
 * @heap: Pointer to the MaxHeap
 */
void heap_destroy(MaxHeap *heap)
{
	if (heap)
	{
		if (heap->elements)
			free(heap->elements);
		free(heap);
	}
}

/**
 * swap_containers - Swaps two Container items in memory
 * @a: Pointer to first Container
 * @b: Pointer to second Container
 */
void swap_containers(Container *a, Container *b)
{
	Container temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * sift_up - Restores Max-Heap property upwards from child to ancestors
 * @heap: Pointer to the MaxHeap
 * @index: Index of the newly placed or modified element
 */
void sift_up(MaxHeap *heap, int index)
{
	int parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (heap->elements[index].priority > heap->elements[parent].priority)
		{
			swap_containers(&heap->elements[index], &heap->elements[parent]);
			index = parent;
		}
		else
		{
			break;
		}
	}
}

/**
 * sift_down - Restores Max-Heap property downwards from parent to descendants
 * @heap: Pointer to the MaxHeap
 * @index: Root index of the subtree to heapify
 */
void sift_down(MaxHeap *heap, int index)
{
	int largest;
	int left;
	int right;

	while (index < heap->size)
	{
		largest = index;
		left = 2 * index + 1;
		right = 2 * index + 2;

		if (left < heap->size && heap->elements[left].priority > heap->elements[largest].priority)
			largest = left;

		if (right < heap->size && heap->elements[right].priority > heap->elements[largest].priority)
			largest = right;

		if (largest != index)
		{
			swap_containers(&heap->elements[index], &heap->elements[largest]);
			index = largest;
		}
		else
		{
			break;
		}
	}
}

/**
 * heap_build - Constructs a Max-Heap from an array of items in O(n) time
 * @heap: Pointer to the MaxHeap
 * @items: Array of Container items
 * @count: Number of items in the array
 */
void heap_build(MaxHeap *heap, const Container items[], int count)
{
	int i;

	if (!heap || !items || count <= 0)
		return;

	if (count > heap->capacity)
	{
		fprintf(stderr, "Error: Item count exceeds heap capacity.\n");
		return;
	}

	for (i = 0; i < count; i++)
		heap->elements[i] = items[i];

	heap->size = count;

	/* Bottom-up heap construction starting from last internal node */
	for (i = (heap->size - 1) / 2; i >= 0; i--)
		sift_down(heap, i);
}

/**
 * heap_insert - Inserts a container into the Max-Heap and restores the property
 * @heap: Pointer to the MaxHeap
 * @item: Container to insert
 *
 * Return: 1 on success, 0 on failure (e.g. overflow)
 */
int heap_insert(MaxHeap *heap, Container item)
{
	if (!heap)
		return (0);

	if (heap->size >= heap->capacity)
	{
		/* Dynamically expand capacity if needed */
		int new_cap = heap->capacity * 2;
		Container *new_arr = (Container *)realloc(heap->elements, new_cap * sizeof(Container));
		if (!new_arr)
		{
			fprintf(stderr, "Error: Heap overflow and memory reallocation failed.\n");
			return (0);
		}
		heap->elements = new_arr;
		heap->capacity = new_cap;
	}

	heap->elements[heap->size] = item;
	heap->size++;
	sift_up(heap, heap->size - 1);

	return (1);
}

/**
 * heap_extract_max - Removes and returns the container with maximum priority
 * @heap: Pointer to the MaxHeap
 * @extracted: Pointer to store the extracted Container
 *
 * Return: 1 on success, 0 if heap is empty
 */
int heap_extract_max(MaxHeap *heap, Container *extracted)
{
	if (!heap || heap->size <= 0)
		return (0);

	if (extracted)
		*extracted = heap->elements[0];

	heap->elements[0] = heap->elements[heap->size - 1];
	heap->size--;

	if (heap->size > 0)
		sift_down(heap, 0);

	return (1);
}

/**
 * heap_remove_by_id - Removes an arbitrary container by its ID and re-heapifies
 * @heap: Pointer to the MaxHeap
 * @id: Identifier string of container to remove
 * @removed: Pointer to store the removed Container
 *
 * Return: 1 if found and removed, 0 if not found
 */
int heap_remove_by_id(MaxHeap *heap, const char *id, Container *removed)
{
	int i;
	int target_idx = -1;

	if (!heap || !id || heap->size <= 0)
		return (0);

	for (i = 0; i < heap->size; i++)
	{
		if (strcmp(heap->elements[i].id, id) == 0)
		{
			target_idx = i;
			break;
		}
	}

	if (target_idx == -1)
		return (0);

	if (removed)
		*removed = heap->elements[target_idx];

	if (target_idx == heap->size - 1)
	{
		heap->size--;
		return (1);
	}

	heap->elements[target_idx] = heap->elements[heap->size - 1];
	heap->size--;

	/* Re-heapify: could need either sift_down or sift_up */
	sift_down(heap, target_idx);
	sift_up(heap, target_idx);

	return (1);
}

/**
 * is_valid_max_heap - Validates that the heap-order property holds for all nodes
 * @heap: Pointer to the MaxHeap
 *
 * Return: 1 if strictly valid, 0 otherwise
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

		if (left < heap->size && heap->elements[left].priority > heap->elements[i].priority)
			return (0);

		if (right < heap->size && heap->elements[right].priority > heap->elements[i].priority)
			return (0);
	}
	return (1);
}

/**
 * print_heap_array - Prints the tabular array representation of the heap
 * @heap: Pointer to the MaxHeap
 * @title: Section banner text
 */
void print_heap_array(const MaxHeap *heap, const char *title)
{
	int i;

	printf("================================================================================\n");
	printf(" %s\n", title);
	printf(" (Size: %d, Valid Max-Heap: %s)\n",
	       heap->size, is_valid_max_heap(heap) ? "YES [OK]" : "NO [VIOLATION]");
	printf("--------------------------------------------------------------------------------\n");
	printf(" %-6s | %-4s | %-24s | %-8s | %-12s\n",
	       "Index", "ID", "Container Name", "Priority", "Children");
	printf("--------------------------------------------------------------------------------\n");

	for (i = 0; i < heap->size; i++)
	{
		int l = 2 * i + 1;
		int r = 2 * i + 2;
		char children_buf[32] = "None";

		if (l < heap->size && r < heap->size)
			snprintf(children_buf, sizeof(children_buf), "L:%s(%d) R:%s(%d)",
			         heap->elements[l].id, heap->elements[l].priority,
			         heap->elements[r].id, heap->elements[r].priority);
		else if (l < heap->size)
			snprintf(children_buf, sizeof(children_buf), "L:%s(%d)",
			         heap->elements[l].id, heap->elements[l].priority);

		printf(" [%2d]   | %-4s | %-24s | %-8d | %s\n",
		       i, heap->elements[i].id, heap->elements[i].name,
		       heap->elements[i].priority, children_buf);
	}
	printf("================================================================================\n\n");
}

/**
 * print_heap_tree - Visualizes the binary tree structure of the heap
 * @heap: Pointer to the MaxHeap
 */
void print_heap_tree(const MaxHeap *heap)
{
	int level = 0;
	int level_nodes = 1;
	int count = 0;
	int i;

	printf("--- Binary Tree Level-by-Level Breakdown ---\n");
	while (count < heap->size)
	{
		printf(" Level %d: ", level);
		for (i = 0; i < level_nodes && count < heap->size; i++, count++)
		{
			printf("[%s:%d] ", heap->elements[count].id, heap->elements[count].priority);
		}
		printf("\n");
		level++;
		level_nodes *= 2;
	}
	printf("--------------------------------------------\n\n");
}
