// ECE 312 - Linked List Lab
// Name: <Your Name Here>
// EID:  <Your EID Here>

#include <stdlib.h>
#include "linkedlist.h"

/*
 * Creates a new empty linked list.
 * 
 * Returns: Pointer to the new LinkedList, or NULL on failure
 */
LinkedList* createList(void) {
    // TODO: Implement this function


	LinkedList* temp = (LinkedList*) calloc (1, sizeof(LinkedList));
	if (temp == NULL) {
		return NULL;
	}

	temp->head = NULL;
	temp->size = 0;
	return temp;
}

/*
 * Frees all memory associated with the linked list.
 *
 * Returns: Nothing
 */
void destroyList(LinkedList* list) {
    // TODO: Implement this function

	Node* next = NULL;

	if (list == NULL) {
		return;
	}

	Node* curr = list->head;
	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}

	free(list);
	return;
}

/*
 * Inserts a new element at the front (head) of the list.
 * 
 * Returns: 0 on success, -1 on failure
 */
int insertAtHead(LinkedList* list, int value) {
    // TODO: Implement this function

	if (list == NULL) {
		return -1;
	}

	Node* newNode = (Node*) calloc(1, sizeof(Node));
	if (newNode == NULL) {
		return -1;
	}

	newNode->next = list->head;
	list->head = newNode;
	list->size++;
	newNode->data = value;
	return 0;
}

/*
 * Inserts a new element at the end (tail) of the list.
 *
 * Returns: 0 on success, -1 on failure
 */
int insertAtTail(LinkedList* list, int value) {
    // TODO: Implement this function

	if (list == NULL) {
		return -1;
	}

	Node* newNode = (Node*) calloc(1, sizeof(Node));

	if (newNode == NULL) {
		return -1;
	}

	newNode->next = NULL;
	newNode->data = value;

	if (list->head == NULL) {
		list->head = newNode;
		list->size++; 				// If the list is empty
	} else {						// If the list is not empty
		Node* curr = list->head;

		while (curr->next != NULL) {
			curr = curr->next;
		}

		curr->next = newNode;
		list->size++;
	}
	return 0;
}

/*
 * Inserts a new element at the specified index.
 *
 * Returns: 0 on success, -1 if index invalid or allocation fails
 */
int insertAtIndex(LinkedList* list, int index, int value) {
    // TODO: Implement this function
    // Consider: What if index == 0? What if index == size?
    // Can you reuse insertAtHead or insertAtTail?

	if (list == NULL) {
		return -1;
	}

	if (index < 0 || index > list->size) {
	return -1;
	}

	if (index == 0) {
	return insertAtHead(list, value);
	}

	if (index == list->size) {
	return insertAtTail(list, value);
	}

	Node* newNode = (Node*) calloc(1, sizeof(Node));

	if (newNode == NULL) {
	return -1;
	}

	newNode->data = value;
	int i;
	Node* curr = list->head;

	for (i = 0; i < index - 1; i++) {
	curr = curr->next;
	}

	newNode->next = curr->next;
	curr->next = newNode;
	list->size++;
	return 0;
}

/*
 * Removes the element at the front (head) of the list.
 *
 * Returns: 0 on success, -1 if list is empty
 */
int removeAtHead(LinkedList* list) {
    // TODO: Implement this function

	if (list == NULL) {
		return -1;
	}

	if (list->head == NULL) {
		return -1;
	}

	Node* temp = list->head;
	list->head = temp->next;
	free(temp);
	list->size--;
	return 0;
}

/*
 * Removes the element at the specified index.
 * 
 * Returns: 0 on success, -1 if index is out of bounds
 */
int removeAtIndex(LinkedList* list, int index) {
    // TODO: Implement this function

    // Consider: What if index == 0? Can you reuse removeAtHead?

	if (list == NULL) {
		return -1;
	}

	if (index < 0 || (index > list->size - 1)) {
		return -1;
	}

	if (index == 0) {
		return removeAtHead(list);
	}

	int i;
	Node* curr = list->head;
	Node* nodeDelete = NULL;

	for (i = 0; i < index - 1; i++) {
		curr = curr->next;
	}

	nodeDelete = curr->next;
	curr->next = nodeDelete->next;
	free(nodeDelete);
	list->size--;
	return 0;
}

/*
 * Retrieves the element at the specified index.
 *
 * Returns: 0 on success, -1 if index out of bounds or NULL pointers
 */
int getElement(LinkedList* list, int index, int* result) {
    // TODO: Implement this function

	if (list == NULL) {
		return -1;
	}

	if (index < 0 || index > (list->size - 1)) {
		return -1;
	}

	int i;
	Node* curr = list->head;

	for (i = 0; i < index; i++) {
		curr = curr->next;
	}

	*result = curr->data;
	return 0;
}

/*
 * Returns the number of elements in the list.
 *
 * Returns -1 if list is NULL.
 */
int getSize(LinkedList* list) {
    // TODO: Implement this function

	if (list == NULL) {
    return -1;
	}

	return list->size;
}
