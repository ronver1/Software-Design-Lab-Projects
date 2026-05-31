#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "lab4.h"

/* ----------------------------------------------------------------
 * ds.c  --  all data structures for the Tech Support Diagnosis Tool
 *
 * Implement every function marked TODO.  The only functions in this
 * entire lab permitted to use recursion are free_tree and count_nodes.
 * Everything else must be iterative.
 * ---------------------------------------------------------------- */


/* ====== Tree nodes ============================================== */

/* TODO 1 */
Node *create_question_node(const char *question) {

	if (question == NULL) {
		return NULL;
	}

	Node* ptr = (Node*) calloc(1, sizeof(Node));

	if (ptr == NULL) {
		return NULL;
	}

	char* temp = strdup(question);
	
	if (temp == NULL) {
		free(ptr);
		return NULL;
	}

	ptr->text = temp;
	ptr->isQuestion = 1;
	ptr->yes = NULL;
	ptr->no = NULL;

	return ptr;
}


/* TODO 2 */
Node *create_solution_node(const char *solution) {

	if (solution == NULL) {
		return NULL;
	}

	Node* ptr = (Node*) calloc(1, sizeof(Node));

	if (ptr == NULL) {
		return NULL;
	}

	char* temp = strdup(solution);

	if (temp == NULL) {
		free(ptr);
		return NULL;
	}

	ptr->text = temp;
	ptr->yes = NULL;
	ptr->no = NULL;
	ptr->isQuestion = 0;

	return ptr;
}

/* TODO 3  (recursion allowed) */
void free_tree(Node *node) {

	if (node == NULL) {
		return;

	} else {
		free_tree(node->no);
		free_tree(node->yes);
		
		free(node->text);
		free(node);
	}

	return;
}

/* TODO 4  (recursion allowed) */
int count_nodes(Node *root) {

	if (root == NULL) {
	    return 0;

	} else {
		return 1 + count_nodes(root->no) + count_nodes(root->yes);
	}

}


/* ====== FrameStack  (dynamic array, iterative traversal) ======== */

/* TODO 5 */
void fs_init(FrameStack *s) {

	if (s == NULL) {
		return;
	}

	s->frames = NULL;
	s->size = 0;
	s->capacity = 0;

}

/* TODO 6 */
void fs_push(FrameStack *s, Node *node, int answeredYes) {

	if (s->capacity == 0) {
		Frame* ptr = (Frame*) calloc(5, sizeof(Frame));

		if (ptr == NULL) {
			return;
		}
		s->frames = ptr;
		s->capacity = 5;
		s->size = 0;
	}

	if (s->size >= s->capacity) {
		Frame* ptr1 = (Frame*) realloc(s->frames, s->capacity*2*sizeof(Frame));
		if (ptr1 == NULL) {
			return;
		}
		s->frames = ptr1;
		s->capacity *= 2;

	}

	s->frames[s->size].node = node;
	s->frames[s->size].answeredYes = answeredYes;
	s->size++;

}

/* TODO 7 */
Frame fs_pop(FrameStack *s) {
    
	Frame temp;

	if (s->size == 0) {
		temp.node = NULL;
		temp.answeredYes = -1;
		
	} else {

	temp = s->frames[s->size - 1];
	s->size--;

	}

	return temp;

}

/* TODO 8 */
int fs_empty(FrameStack *s) {

	if (s->size == 0) {
		return 1;
	} 

	return 0;

}

/* TODO 9 */
void fs_free(FrameStack *s) {
	
	free(s->frames);

	s->frames = NULL;
	s->size = 0;
	s->capacity = 0;
}


/* ====== EditStack  (dynamic array, undo/redo) =================== */

/* TODO 10 */
void es_init(EditStack *s) {

	if (s == NULL) {
		return;
	}

	s->edits = NULL;
	s->size = 0;
	s->capacity = 0;
	
	return;
}

/* TODO 11 */
void es_push(EditStack *s, Edit e) {

	if (s->capacity == 0) {

		Edit* temp = (Edit*) calloc(5, sizeof(Edit));

		if (temp == NULL) {
			return;
		}

		s->edits = temp;
		s->size = 0;
		s->capacity = 5;
	}

	if (s->size >= s->capacity) {
		
		Edit* ptr = (Edit*) realloc(s->edits, s->capacity * 2);

		if (ptr == NULL) {
			return; 
		}

		s->edits = ptr;
		s->capacity *= 2;
	}

	s->edits[s->size] = e;

	s->size++;

	return;

}

/* TODO 12 */
Edit es_pop(EditStack *s) {

    Edit temp = {0}; 

    if (s->size == 0) {
	    return temp;
    }

    temp = s->edits[s->size - 1];
    s->size--;
    return temp;

}

/* TODO 13 */
int es_empty(EditStack *s) {

	if (s->size == 0) {
	    return 1;
	} 	

	return 0;
}

/* TODO 14 */
void es_clear(EditStack *s) {

	if (s == NULL) {
		return;
	}

	s->size = 0;

}

/* provided -- do not modify */
void es_free(EditStack *s) {
    free(s->edits);
    s->edits    = NULL;
    s->size     = 0;
    s->capacity = 0;
}

void free_edit_stack(EditStack *s) { es_free(s); }


/* ====== Queue  (linked list, BFS) ============================== */

/* TODO 15 */
void q_init(Queue *q) {

	if (q == NULL) {
		return;
	}

	q->front = NULL;
	q->rear = NULL;
	q->size = 0;

	return;

}

/* TODO 16 */
void q_enqueue(Queue *q, Node *node, int id) {

	if (q == NULL || node == NULL) {
		return;
	}

	QueueNode* ptr = (QueueNode*) calloc(1, sizeof(QueueNode));

	if (ptr == NULL) {
		return;
	}

	ptr->treeNode = node;
	ptr->id = id;
	ptr->next = NULL;

	if (q->size == 0) {
		q->front = ptr;
		q->rear = ptr;
	} else {

	QueueNode* temp = q->rear;
	q->rear = ptr;
	temp->next = ptr;

	}

	q->size++;
	return;

}

/* TODO 17 */
int q_dequeue(Queue *q, Node **node, int *id) {

    if (q == NULL || q->size == 0) {
	    return 0;
    }

    QueueNode* ptr = q->front;
    *node = ptr->treeNode;
    *id = ptr->id;

    if (q->size == 1) {
	    q->front = NULL;
	    q->rear = NULL;
    } else {
	    q->front = ptr->next;
    }

    free(ptr);
    q->size--;
    return 1;

}

/* TODO 18 */
int q_empty(Queue *q) {

	if (q == NULL || q->size == 0) {
		return 1;
	}

	return 0;

}

/* TODO 19 */
void q_free(Queue *q) {

	if (q == NULL) {
		return;
	}

	QueueNode* curr = q->front;
	QueueNode* next;

	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}

	q->front = NULL;
	q->rear = NULL;
	q->size = 0;
	return;
}


/* ====== Hash table  (separate chaining) ======================== */

/* TODO 20
 * Convert a string to a canonical key:
 *   letters  -> lowercase
 *   spaces   -> underscore
 *   anything else -> drop
 * Caller owns the returned string and must free() it.
 */
char *canonicalize(const char *s) {
	
	if (s == NULL) {
		return NULL;
	}

	int inputLength = strlen(s);

	char* result = (char*) calloc(inputLength + 1, sizeof(char));

	if (result == NULL) {
		return NULL;
	}
	int resultIndex = 0;

	for (int i = 0; i < inputLength; i++) {
		
		if (s[i] >= 0x41 && s[i] <= 0x5A) {
			result[resultIndex] = s[i] + 32;
			resultIndex++;
		}

		if (s[i] == 0x20) {
			result[resultIndex] = 0x5F;
			resultIndex++;
		}

		if (s[i] >= 0x61 && s[i] <= 0x7A) {
			result[resultIndex] = s[i];
			resultIndex++;
		}
	}

	result[resultIndex] = 0;

	return result;

}

/* TODO 21  (djb2: hash = hash*33 + c, seed 5381) */
unsigned h_hash(const char *s) {

	unsigned hash = 5381;

	unsigned inputLength = strlen(s);

	for (unsigned i = 0; i < inputLength; i++) {
		hash = (hash*33) + s[i];
	}

	return hash;

}

/* TODO 22 */
void h_init(Hash *h, int nbuckets) {

	if (h == NULL) {
		return;
	}

	Entry** ptr = (Entry**) calloc(nbuckets, sizeof(Entry*));

	if (ptr == NULL) {
		return;
	}

	h->buckets = ptr;
	h->nbuckets = nbuckets;
	h->size = 0;

}

/* TODO 23 */
int h_put(Hash *h, const char *key, int solutionId) {

	if (h == NULL || key == NULL) {
		return 0;
	}

	unsigned hash = h_hash(key);
	unsigned index = hash % (unsigned)h->nbuckets;

	int found = 0;
	Entry* curr = h->buckets[index];
	while (curr != NULL && found == 0) {
		int x = strcmp(curr->key, key);

		if (x == 0 && found == 0) {
			found = 1;
		} else {
			curr = curr->next;
		}
	}

	if (found == 1) {

		int* currId = curr->vals.ids;
		int i = 0;
		int match = 0;

		for (i = 0; i < curr->vals.count; i++) {
			if (currId[i] == solutionId) {
				match = 1;
			}
		}

	if (match == 0) {

		if (curr->vals.count >= curr->vals.capacity) {
			int* ptr = (int*) realloc(currId, 2*curr->vals.capacity * sizeof(int));

			if (ptr == NULL) {
				return 0;
			}

			curr->vals.capacity *= 2;
			curr->vals.ids = ptr;
		}

			curr->vals.ids[i] = solutionId;
			curr->vals.count++;
			return 1;
	}

	if (match == 1) {
		return 1;
		}

	}

	if (found == 0) {
		Entry* ptr0 = (Entry*) calloc(1, sizeof(Entry));

		if (ptr0 == NULL) {
			return 0;
		}

		char* temp = strdup(key);

		if (temp == NULL) {
			free(ptr0);
			return 0;
		}

		ptr0->key = temp;

		int* temp0 = (int*) calloc(5, sizeof(int));

		if (temp0 == NULL) {
			free(ptr0);
			free(temp);
			return 0;
		}


		ptr0->vals.ids = temp0;
		temp0[0] = solutionId;
		ptr0->vals.count = 1;
		ptr0->vals.capacity = 5;
		
		Entry* temp1 = h->buckets[index];
		ptr0->next = temp1;
		h->buckets[index] = ptr0;

		h->size++;
		return 1;
	}
	return 0;
}


/* TODO 24 */
int h_contains(const Hash *h, const char *key, int solutionId) {
    
	if (h == NULL || key == NULL) {
		return 0;
	}

	unsigned hash = h_hash(key);
	unsigned index = hash % (unsigned)h->nbuckets;

	int found = 0;
	Entry* curr = h->buckets[index];
	while (curr != NULL && found == 0) {
		int x = strcmp(curr->key, key);

		if (x == 0 && found == 0) {
			found = 1;
		} else {
			curr = curr->next;
		}
	}

	if (found == 0) {
		return 0;
	}

	int match = 0;
	if (found == 1) {

		int* currId = curr->vals.ids;
		int i = 0;

		for (i = 0; i < curr->vals.count; i++) {
			if (currId[i] == solutionId) {
				match = 1;
			}
		}
	}	

	if (found == 1 && match ==1) {
		return 1;
	} else  {
		return 0;
	}
}


/* TODO 25 */
int *h_get_ids(const Hash *h, const char *key, int *outCount) {

	if (h == NULL || key == NULL) {
		*outCount = 0;
		return NULL;
	}

	unsigned hash = h_hash(key);
	unsigned index = hash % (unsigned)h->nbuckets;

	int found = 0;
	Entry* curr = h->buckets[index];
	while (curr != NULL && found == 0) {
		int x = strcmp(curr->key, key);

		if (x == 0 && found == 0) {
			found = 1;
		} else {
			curr = curr->next;
		}
	}

	if (found == 0) {
		*outCount = 0;
		return NULL;
	} else {
		*outCount = curr->vals.count;
    	return curr->vals.ids;
	}
}

/* TODO 26 */
void h_free(Hash *h) {

	if (h == NULL) {
		return;
	}

	Entry* curr = NULL;
	Entry* next = NULL;

	for (int i = 0; i < h->nbuckets; i++) {
		curr = h->buckets[i];
		while (curr != NULL) {
			next = curr->next;
			free(curr->vals.ids);
			free(curr->key);
			free(curr);
			curr = next;
		}
	}

	free(h->buckets);
	h->buckets = NULL;
	h->nbuckets = 0;
	h->size = 0;
	return;

}
