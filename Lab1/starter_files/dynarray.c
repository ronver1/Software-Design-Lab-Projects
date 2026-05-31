// ECE 312 - Dynamic Array Lab
// Name: Ronit Verma
// EID:  RV24923

#include <stdlib.h>
#include "dynarray.h"

/*
 * Creates a new DynamicArray with the specified initial capacity.
 * Returns: Pointer to the new DynamicArray, or NULL on failure
 */
DynamicArray* createArray(int initialCapacity) {
    // TODO: Implement this function

	// Define variables needed for this function
	int* Pointer_1 = NULL;
	DynamicArray* dyn = NULL;

	// Function
	if (initialCapacity <= 0) {
		return NULL;
	} else {
		dyn = (DynamicArray*) calloc(1, sizeof(DynamicArray));
	}

	if (dyn == NULL) {
		free(dyn);
		return NULL;
	} else {
		Pointer_1 = (int*) calloc(initialCapacity, sizeof(int));
	}

	if (Pointer_1 == NULL) {
		free(dyn);
		return NULL;
	} else {
		dyn -> size = 0;
		dyn -> capacity = initialCapacity;
		dyn -> data = Pointer_1;
		return dyn;
	}
}

/*
 * Frees all memory associated with the DynamicArray.
 */
void destroyArray(DynamicArray* arr) {
    // TODO: Implement this function

	if (arr == NULL) {
		return;
	}

	if (arr->data != NULL) {
		free(arr->data);
	}

	free(arr);

}

/*

 * Adds an element to the end of the array.
 * Returns: 0 on success, -1 on failure
 */
int addElement(DynamicArray* arr, int value) {
    // TODO: Implement this function

	// Define variable needed for this function

	// Function

	int* newData = NULL;

	if (arr == NULL) {
		return -1;
	}

	int size = arr->size;
	int capacity = arr->capacity;

	if (size < capacity) {
		arr->data[size] = value;
		arr->size = size + 1;
		return 0;
	}

	if (capacity > 0) {
		capacity = capacity * 2;
		newData = (int*)realloc(arr->data, capacity*sizeof(int));
		if (newData == NULL) {
			return -1;
		} else {
			arr->data = newData;
			arr->capacity = capacity;
			}

		}

 	if (capacity == 0) {
		capacity = 1;
		newData = (int*)realloc(arr->data, capacity*sizeof(int));
		if (newData == NULL) {
			return -1;
		} else {
			arr->data = newData;
			arr->capacity = capacity;
			}
		}

			arr->data[size] = value;
			arr->size = arr->size + 1;
			return 0;
}

/*
 * Retrieves the element at the specified index.
 * Returns: 0 on success, -1 on failure (invalid index or NULL pointers)
 */
int getElement(DynamicArray* arr, int index, int* result) {
    // TODO: Implement this function

	if (arr == NULL) {
	return -1;
	}

	if (result == NULL) {
	return -1;
	}

	if (index < 0 || index >= arr->size) {
	return -1;
	}

	*result = arr->data[index];
	return 0;
}

/*
 * Sets the element at the specified index to the given value.
 * Returns: 0 on success, -1 on failure
 */
int setElement(DynamicArray* arr, int index, int value) {
    // TODO: Implement this function

	if (arr == NULL) {
		return -1;
	}

	if (index < 0 || index >= arr->size) {
		return -1;
	}

	arr->data[index] = value;
	return 0;
}

/*
 * Returns the current number of elements in the array.
 * Returns -1 if arr is NULL.
 */
int getSize(DynamicArray* arr) {
    // TODO: Implement this function

	int a = 0;

	if (arr == NULL) {
		return -1;
	}

	a = arr->size;
	return a;
}

/*
 * Returns the current capacity of the array.
 * Returns -1 if arr is NULL.
 */
int getCapacity(DynamicArray* arr) {
    // TODO: Implement this function

	int a = 0;

	if (arr == NULL) {
		return -1;
	}

	a = arr->capacity;
	return a;
}

/*
 * Removes the element at the specified index.
 * Returns: 0 on success, -1 on failure
 */
int removeElement(DynamicArray* arr, int index) {
    // TODO: Implement this function

	int i;

	if (arr == NULL) {
		return -1;
	}

	if (arr->size == 0) {
		return -1;
	}

	if (index < 0 || index >= arr->size) {
		return -1;
	}

	for (i = index; i < arr->size - 1; i++) {
		arr->data[i] = arr->data[i + 1];
	}

	arr->size = arr->size - 1;
	return 0;
}
