#ifndef BAGGAGE_HEAP_H
#define BAGGAGE_HEAP_H

#include <stddef.h>

#define MAX_NAME_LEN 64
#define MAX_ID_LEN 8

/**
 * struct Container - Represents a baggage container in the handling system
 * @id: Unique container identifier (e.g., "A", "B", ..., "X")
 * @name: Descriptive name or flight designation (e.g., "Cargo-A", "Urgent-X")
 * @priority: Priority score assigned by the baggage handling system
 */
typedef struct
{
	char id[MAX_ID_LEN];
	char name[MAX_NAME_LEN];
	int priority;
} Container;

/**
 * struct MaxHeap - Dynamic array-based binary Max-Heap for containers
 * @elements: Pointer to contiguous array of Container structs
 * @size: Current number of elements in the heap
 * @capacity: Maximum capacity allocated for the heap
 */
typedef struct
{
	Container *elements;
	int size;
	int capacity;
} MaxHeap;

/* Heap creation and destruction */
MaxHeap *heap_create(int capacity);
void heap_destroy(MaxHeap *heap);

/* Core heap operations */
void heap_build(MaxHeap *heap, const Container items[], int count);
int heap_insert(MaxHeap *heap, Container item);
int heap_extract_max(MaxHeap *heap, Container *extracted);
int heap_remove_by_id(MaxHeap *heap, const char *id, Container *removed);

/* Internal heapification */
void sift_up(MaxHeap *heap, int index);
void sift_down(MaxHeap *heap, int index);
void swap_containers(Container *a, Container *b);

/* Verification and visualization */
int is_valid_max_heap(const MaxHeap *heap);
void print_heap_array(const MaxHeap *heap, const char *title);
void print_heap_tree(const MaxHeap *heap);

#endif /* BAGGAGE_HEAP_H */
