#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "lab4.h"

extern Node *g_root;

/* ----------------------------------------------------------------
 * TODO 29  check_integrity
 *
 * Use BFS to verify:
 *   - Every question node has both yes and no children (non-NULL).
 *   - Every solution node has both children NULL.
 * Return 1 if valid, 0 if any violation is found.
 * ---------------------------------------------------------------- */
int check_integrity(void) {

    if (g_root == NULL) {
        return 1;
    }

    Queue q;
    q_init(&q);
    uint32_t idCount = 0;
    q_enqueue(&q, g_root, idCount);

    Node* curr = NULL;
    int currId;
    while (q.front != NULL) {
        q_dequeue(&q, &curr, &currId);
        if (curr->isQuestion == 1) {
            if (curr->yes == NULL || curr->no == NULL) {
                q_free(&q);
                return 0;
            }
        } else {
            if (curr->yes != NULL || curr->no != NULL) {
                q_free(&q);
                return 0;
            }
        }
        if (curr->yes != NULL) {
            q_enqueue(&q, curr->yes, idCount);
            idCount++;
        }
        if (curr->no != NULL) {
            q_enqueue(&q, curr->no, idCount);
            idCount++;
        }
    }
    q_free(&q);
    return 1;
}

/* ----------------------------------------------------------------
 * TODO 30  find_shortest_path
 *
 * Given the exact text of two solution leaves, display the
 * questions that distinguish them.  Use BFS with a parent-tracking
 * PathNode array to find both leaves, build ancestor arrays for
 * each, find the Lowest Common Ancestor (LCA), then print:
 *   - The shared path of questions both solutions pass through.
 *   - The divergence question (LCA) and which branch leads where.
 *
 * Display results with mvprintw.  Print an error if either
 * solution is not found.  Free all allocations before returning.
 * ---------------------------------------------------------------- */

typedef struct PathNode{
    Node* node;
    int parentId;
    int id;
} PathNode;

void find_shortest_path(const char *sol1, const char *sol2) {

    if (g_root == NULL) {
        mvprintw(10, 2, "Error: knowledge base is empty.");
        refresh();
        return;
    }

    int nodeAmt = count_nodes(g_root);
    PathNode* ptr = (PathNode*) calloc(nodeAmt, sizeof(PathNode));
    if (ptr == NULL) {
        return;
    }

    Queue q;
    q_init(&q);
    int ptrCount = 0;
    q_enqueue(&q, g_root, -1);      
    
    Node* curr = NULL;
    int currId;

    while (q.front != NULL) {
        q_dequeue(&q, &curr, &currId);
        ptr[ptrCount].node = curr;
        ptr[ptrCount].id = ptrCount;
        ptr[ptrCount].parentId = currId;

        if (curr->yes != NULL){
            q_enqueue(&q, curr->yes, ptrCount);
        }
        if (curr->no != NULL) {
            q_enqueue(&q, curr->no, ptrCount);
        }

        ptrCount++;
    }
    q_free(&q);

    int i;
    int* path1 = (int*) calloc(nodeAmt, sizeof(int));
    if (path1 == NULL) {
        free(ptr);
        return;
    }
    int* path2 = (int*) calloc(nodeAmt, sizeof(int));
    if (path2 == NULL) {
        free(ptr);
        free(path1);
        return;
    }


    int sol1Index = -1;
    for (i = 0; i < ptrCount; i++) {
        if (strcmp(ptr[i].node->text, sol1) == 0) {
            sol1Index = i;
            break;
        }
    }

    if (sol1Index == -1) {
        mvprintw(10, 2, "Error: Option 1 not Found in Tree");
        free(path1); 
        free(path2);
        free(ptr);
        refresh();
        return;
    }

    int path1Index = 0;
    int current = sol1Index;
    while (current != -1) {
        path1[path1Index] = current;
        path1Index++;
        current = ptr[current].parentId;
    }

    int sol2Index = -1;
    for (i = 0; i < ptrCount; i++) {
        if (strcmp(ptr[i].node->text, sol2) == 0) {
            sol2Index = i;
            break;
        }
    }

    if (sol2Index == -1) {
        mvprintw(10, 2, "Error: Option 2 not Found in Tree");
        free(path1); 
        free(path2);
        free(ptr);
        refresh();
        return;
    }

    int path2Index = 0;
    current = sol2Index;
    while (current != -1) {
        path2[path2Index] = current;
        path2Index++;
        current = ptr[current].parentId;
    }

    i = path1Index - 1;
    int j = path2Index - 1;
    int LCAid = -1;

    while (i >= 0 && j >= 0) {
        if (path1[i] == path2[j]) {
            LCAid = path1[i];
            i--;
            j--;
        } else {
            break;
        }
    }

    int row = 10;

    if (LCAid == -1) {
        mvprintw(row++, 2, "Error: No common ancestor found.");
        free(path1); free(path2); free(ptr);
        refresh();
        return;
    }

    mvprintw(row++, 2, "Shared path:");
    for (int k = path1Index - 1; k > i + 1; k--) {
        mvprintw(row++, 4, "Q: %s", ptr[path1[k]].node->text);
    }
    
    mvprintw(row++, 2, "Divergence point:");
    mvprintw(row++, 4, "Q: %s", ptr[LCAid].node->text);

    if (ptr[LCAid].node->yes == ptr[path1[i]].node) {
        mvprintw(row++, 4, "YES -> %s", sol1);
        mvprintw(row++, 4, "NO  -> %s", sol2);
    } else {
     mvprintw(row++, 4, "NO  -> %s", sol1);
        mvprintw(row++, 4, "YES -> %s", sol2);
    }
    refresh();

    free(path1);
    free(path2);
    free(ptr);
    return;

}
