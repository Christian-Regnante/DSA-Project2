#ifndef TRIAGE_HEAP_H
#define TRIAGE_HEAP_H

#include <stddef.h>

#define MAX_NAME_LEN 64
#define MAX_ID_LEN 16

/**
 * struct Patient - Represents a patient record in the emergency triage system
 * @id: Patient identifier (e.g., "P01", "PO1", "P08")
 * @name: Full name of the patient (e.g., "Amina", "Kofi")
 * @score: Triage priority score (higher score = higher medical urgency)
 */
typedef struct
{
	char id[MAX_ID_LEN];
	char name[MAX_NAME_LEN];
	int score;
} Patient;

/**
 * struct MaxHeap - Contiguous array-based binary Max-Heap for emergency triage
 * @patients: Pointer to contiguous array of Patient records
 * @size: Current number of patients in the heap
 * @capacity: Maximum capacity allocated for the heap
 */
typedef struct
{
	Patient *patients;
	int size;
	int capacity;
} MaxHeap;

/* Lifecycle functions */
MaxHeap *heap_create(int capacity);
void heap_destroy(MaxHeap *heap);

/* Core heap operations */
void heap_build(MaxHeap *heap, const Patient records[], int count);
int heap_insert(MaxHeap *heap, Patient patient);
int heap_extract_max(MaxHeap *heap, Patient *extracted);
int heap_remove_by_id(MaxHeap *heap, const char *id, Patient *removed);

/* Internal heapify operations */
void sift_up(MaxHeap *heap, int index);
void sift_down(MaxHeap *heap, int index);
void swap_patients(Patient *a, Patient *b);

/* Verification and visualization */
int is_valid_max_heap(const MaxHeap *heap);
void print_heap_array(const MaxHeap *heap, const char *title);
void print_heap_tree(const MaxHeap *heap);

#endif /* TRIAGE_HEAP_H */
