#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "lab4.h"

extern Node      *g_root;
extern EditStack  g_undo;
extern EditStack  g_redo;
extern Hash       g_index;

/* ----------------------------------------------------------------
 * TODO 31  run_diagnosis
 *
 * Walk the decision tree iteratively (no recursion) using a
 * FrameStack.  At each question node ask the user yes/no and push
 * the appropriate child.  At each solution leaf display the fix and
 * ask whether it solved the problem.
 *
 * If the fix did not help, enter the learning phase:
 *   - Ask the user what would actually fix the problem.
 *   - Ask for a yes/no question that distinguishes their problem
 *     from the solution just shown.
 *   - Ask which answer applies to their problem.
 *   - Create a new question node and a new solution node, wire them
 *     correctly, graft them into the tree, record an Edit for
 *     undo/redo, and index the new question with canonicalize/h_put.
 *
 * Edge case: if parent is NULL the root itself must be replaced.
 * ---------------------------------------------------------------- */
void run_diagnosis(void) {
    clear();
    attron(COLOR_PAIR(5) | A_BOLD);
    mvprintw(0, 0, "%-80s", " Tech Support Diagnosis");
    attroff(COLOR_PAIR(5) | A_BOLD);

    mvprintw(2, 2, "%s", "I'll help diagnose your tech problem.");
    mvprintw(3, 2, "%s", "Answer each question with y or n.");
    mvprintw(4, 2, "%s", "Press any key to start...");
    refresh();
    getch();

    FrameStack stack;
    fs_init(&stack);
    int row = 5;

    fs_push(&stack, g_root, -1);
    Node *parent = NULL;
    int wasYes = -1;

    while (!fs_empty(&stack)) {

        if (row > 20) {
            row = 6;
        }

        Frame curr = fs_pop(&stack);
        if (curr.node->isQuestion == 1) {
            int yesNo = get_yes_no(row++, 2, curr.node->text); 
                if (yesNo == 1) {
                    parent = curr.node;
                    wasYes = 1;
                    fs_push(&stack, curr.node->yes, 1);
                } else {
                    parent = curr.node;
                    wasYes = 0;
                    fs_push(&stack, curr.node->no, 0);
                }
            }
        
        if (curr.node->isQuestion == 0) {
            mvprintw(row, 2, "%s", curr.node->text);
            refresh();
            row++;
            int fixProb = get_yes_no(row, 2, "Did this fix your problem?");
            refresh();
            row++;
            if (fixProb == 1) {
                mvprintw(row, 2, "%s", "Thank you!");
                refresh();
                break;
            } else {
                char* tempSol = get_input(row++, 2, "What would fix your problem?");
                char* newSol = strdup(tempSol);

                char* tempQ = get_input(row++, 2, "What yes or no question distingushes your problem?");
                char* newQ = strdup(tempQ);
                int newYesNo = get_yes_no(row++, 2, "Is the answer yes or no for your problem?");

                Node* newQuestion = create_question_node(newQ);
                if (newQuestion == NULL) {
                    free(newSol);
                    free(newQ);
                    fs_free(&stack);
                    return;
                }
                free(newQ);
                Node* newSolution = create_solution_node(newSol);
                if (newSolution == NULL) {
                    free(newSol);
                    free_tree(newQuestion);
                    fs_free(&stack);
                    return;
                }
                free(newSol);
                if (newYesNo == 1) {
                    newQuestion->yes = newSolution;
                    newQuestion->no = curr.node;
                } else {
                    newQuestion->yes = curr.node;
                    newQuestion->no = newSolution;
                }

                if (parent == NULL) {
                    g_root = newQuestion;
                } else {
                    if (wasYes == 1) {
                        parent->yes = newQuestion;
                    } else {
                        parent->no = newQuestion;
                    }           
                }

                Edit e; 
                e.type = EDIT_INSERT_SPLIT;
                e.parent = parent;
                e.wasYesChild = wasYes;
                e.oldLeaf = curr.node;
                e.newQuestion = newQuestion;
                e.newLeaf = newSolution;
                es_push(&g_undo, e);
                es_clear(&g_redo);

                char *key = canonicalize(newQuestion->text);
                h_put(&g_index, key, 0);
                free(key);
            }
        }
    }
    fs_free(&stack);
}

/* ----------------------------------------------------------------
 * TODO 32  undo_last_edit
 * Return 1 on success, 0 if the undo stack is empty.
 * ---------------------------------------------------------------- */
int undo_last_edit(void) {

    if (es_empty(&g_undo)) {
        return 0;
    }

    Edit e = es_pop(&g_undo);
    if (e.parent == NULL) {
        g_root = e.oldLeaf;
    } else {
        if (e.wasYesChild == 1) {
            e.parent->yes = e.oldLeaf;
        } else {
            e.parent->no = e.oldLeaf;
        }
    }
    es_push(&g_redo, e);
    return 1;

}


/* ----------------------------------------------------------------
 * TODO 33  redo_last_edit
 * Return 1 on success, 0 if the redo stack is empty.
 * ---------------------------------------------------------------- */
int redo_last_edit(void) {

    if (es_empty(&g_redo)) {
        return 0;
    }

    Edit e = es_pop(&g_redo);
    if (e.parent == NULL) {
        g_root = e.newQuestion;
    } else {
        if (e.wasYesChild == 1) {
            e.parent->yes = e.newQuestion;
        } else {
            e.parent->no = e.newQuestion;
        }
    }
    es_push(&g_undo, e);
    return 1;

}
