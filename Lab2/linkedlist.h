/*
	Name: Ronit Verma
	EID RV24923
	Class: Dr. Speight ECE 312H
*/

	#ifndef LINKEDLIST_H
	#define LINKEDLIST_H

// Define the Linked List Struct and Node Struct

	typedef struct Node {
		int data;
		struct Node* next;
	} Node;

	typedef struct LinkedList {
		Node* head;
		int size;
	} LinkedList;

// Define the functions

	LinkedList* createList(void);
	void destroyList(LinkedList* list);
	int insertAtHead(LinkedList* list, int value);
	int insertAtTail(LinkedList* list, int value);
	int insertAtIndex(LinkedList* list, int index, int value);
	int removeAtHead(LinkedList* list);
	int removeAtIndex(LinkedList* list, int index);
	int getElement(LinkedList* list, int index, int* result);
	int getSize(LinkedList* list);

	#endif

