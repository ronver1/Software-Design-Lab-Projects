/*
	Name: Ronit Verma
	EID: RV24923
	Class: Dr. Speight: ECE 312H
*/

	#ifndef DYNARRAY_H
	#define DYNARRAY_H



// Define the DynamicArray struct //

	typedef struct DynamicArray {
		int* data;
		int size;
		int capacity;
	} DynamicArray;

// Declare the functions //

	DynamicArray* createArray(int initialCapacity);
	void destroyArray(DynamicArray* arr);
	int addElement(DynamicArray* arr, int value);
	int getElement(DynamicArray* arr, int index, int*result);
	int setElement(DynamicArray* arr, int index, int value);
	int getSize(DynamicArray* arr);
	int getCapacity(DynamicArray* arr);
	int removeElement(DynamicArray* arr, int index);

	#endif
