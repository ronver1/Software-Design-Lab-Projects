#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "lab4.h"

extern Node *g_root;

#define MAGIC   0x54454348u   /* "TECH" */
#define VERSION 1u

typedef struct { Node *node; int id; } NodeMapping;

/* ----------------------------------------------------------------
 * TODO 27  save_tree
 *
 * Serialize the entire tree to a binary file using BFS order.
 *
 * File format:
 *   Header:  uint32 magic | uint32 version | uint32 nodeCount
 *   Per node (BFS order):
 *     uint8  isQuestion
 *     uint32 textLen          (bytes, no null terminator in file)
 *     char[] text             (exactly textLen bytes)
 *     int32  yesId            (-1 if NULL)
 *     int32  noId             (-1 if NULL)
 *
 * Return 1 on success, 0 on failure.
 * ---------------------------------------------------------------- */
int save_tree(const char *filename) {
    
    if (g_root == NULL) {
        return 0;
    }

    FILE* fptr = fopen(filename,"wb");
    if (fptr == NULL) {
        return 0;
    }

    NodeMapping* ptr = (NodeMapping*) calloc(count_nodes(g_root), sizeof(NodeMapping));
    if (ptr == NULL) {
        fclose(fptr);
        return 0;
    }

    Queue q;
    q_init(&q);
    uint32_t idCount = 0;
    uint32_t mappingCount = 0;
    q_enqueue(&q, g_root, idCount);
    idCount++;

    Node* curr = NULL;
    int currId;

    while (q.front != NULL) {
        q_dequeue(&q, &curr, &currId);
        ptr[mappingCount].node = curr;
        ptr[mappingCount].id = currId;
        mappingCount++;
        if (curr->yes != NULL){
            q_enqueue(&q, curr->yes, idCount);
            idCount++;
        }
        if (curr->no != NULL) {
            q_enqueue(&q, curr->no, idCount);
            idCount++;
        }
    }

    uint32_t magic = MAGIC;
    uint32_t version = VERSION;
    uint32_t nodeCount = (uint32_t)count_nodes(g_root);
    uint32_t temp = fwrite(&magic, sizeof(uint32_t), 1, fptr);
    if (temp != 1) {
        q_free(&q);
        fclose(fptr);
        free(ptr);
        return 0;
    }
    uint32_t temp0 = fwrite(&version, sizeof(uint32_t), 1, fptr);
    if (temp0 != 1) {
        q_free(&q);
        fclose(fptr);
        free(ptr);
        return 0;
    }
    uint32_t temp1 = fwrite(&nodeCount, sizeof(uint32_t), 1, fptr);
    if (temp1 != 1) {
        q_free(&q);
        fclose(fptr);
        free(ptr);
        return 0;
    }

    uint32_t temp2;
    for (uint32_t i = 0; i < mappingCount; i++) {

        temp2 = fwrite(&ptr[i].node->isQuestion, sizeof(uint8_t), 1, fptr);
        if (temp2 != 1) {
            q_free(&q);
            fclose(fptr);
            free(ptr);
            return 0;
        }

        uint32_t textLen = strlen(ptr[i].node->text);
        temp2 = fwrite(&textLen, sizeof(uint32_t), 1, fptr);
        if (temp2 != 1) {
            q_free(&q);
            fclose(fptr);
            free(ptr);
            return 0;
        }
        temp2 = fwrite(ptr[i].node->text, sizeof(char), textLen, fptr);
        if (temp2 != textLen) {
            q_free(&q);
            fclose(fptr);
            free(ptr);
            return 0;
        }
        
        int32_t yesId = -1;
        if (ptr[i].node->yes != NULL) {
            for (uint32_t j = 0; j < mappingCount; j++) {
                if (ptr[i].node->yes == ptr[j].node) {
                    yesId = ptr[j].id;
                }
            }
        }
        
        temp2 = fwrite(&yesId, sizeof(int32_t), 1, fptr);
        if (temp2 != 1) {
            q_free(&q);
            fclose(fptr);
            free(ptr);
            return 0;
        } 

        int32_t noId = -1;
        if (ptr[i].node->no != NULL) {
            for (uint32_t j = 0; j < mappingCount; j++) {
                if (ptr[i].node->no == ptr[j].node) {
                    noId = ptr[j].id;
                }
            }
        }
        temp2 = fwrite(&noId, sizeof(int32_t), 1, fptr);
        if (temp2 != 1) {
            q_free(&q);
            fclose(fptr);
            free(ptr);
            return 0;
        }
    }

    q_free(&q);
    free(ptr);
    fclose(fptr);
    return 1;

}

/* ----------------------------------------------------------------
 * TODO 28  load_tree
 *
 * Read a file written by save_tree and reconstruct the tree.
 * Validate the magic number.  Read all nodes into a flat array
 * first, then link children in a second pass.
 * Free any existing g_root before installing the new one.
 * Return 1 on success, 0 on any error (free partial allocations).
 * ---------------------------------------------------------------- */
int load_tree(const char *filename) {

    FILE* fptr = fopen(filename,"rb");
    if (fptr == NULL) {
        return 0;
    }
    
    uint32_t magic;
    uint32_t version;
    uint32_t nodeCount;
    size_t temp0;

    temp0 = fread(&magic, sizeof(uint32_t), 1, fptr);
    if (temp0 != 1) {
        fclose(fptr);
        return 0;
    }

    temp0 = fread(&version, sizeof(uint32_t), 1, fptr);
    if (temp0 != 1) {
        fclose(fptr);
        return 0;
    }

    temp0 = fread(&nodeCount, sizeof(uint32_t), 1, fptr);
    if (temp0 != 1) {
        fclose(fptr);
        return 0;
    }

    if (magic != MAGIC) {
        fclose(fptr);
        return 0;
    }

    Node** nodePtr = (Node**) calloc(nodeCount, sizeof(Node*));
    if (nodePtr == NULL) {
        fclose(fptr);
        return 0;
    }

    int32_t* yesPtr = (int32_t*) calloc(nodeCount, sizeof(int32_t));
    if (yesPtr == NULL) {
        free(nodePtr);
        fclose(fptr);
        return 0;
    }

    int32_t* noPtr = (int32_t*) calloc(nodeCount, sizeof(int32_t));
    if (noPtr == NULL) {
        free(yesPtr);
        free(nodePtr);
        fclose(fptr);
        return 0;
    }

    uint8_t isQ;
    uint32_t textLen;
    uint32_t temp1, i;
    for (i = 0; i < nodeCount; i++) {
       temp1 = fread(&isQ, sizeof(uint8_t), 1, fptr);
        if (temp1 != 1) {
            free(yesPtr);
            free(noPtr);
            free(nodePtr);
            fclose(fptr);
            return 0;
        }
        temp1 = fread(&textLen, sizeof(uint32_t), 1, fptr);
        if (temp1 != 1) {
            free(yesPtr);
            free(noPtr);
            free(nodePtr);
            fclose(fptr);
            return 0;
        }
        char* buf = (char*) calloc(textLen + 1, sizeof(char));
        if (buf == NULL) {
            free(yesPtr);
            free(noPtr);
            free(nodePtr);
            fclose(fptr);
            return 0;
        }
        temp1 = fread(buf, sizeof(char), textLen, fptr);
        if (temp1 != textLen) {
            free(yesPtr);
            free(noPtr);
            free(buf);
            free(nodePtr);
            fclose(fptr);
            return 0;
        }
        buf[textLen] = '\0';

        if (isQ == 1) {
            nodePtr[i] = create_question_node(buf);
            free(buf);
        } else {
            nodePtr[i] = create_solution_node(buf);
            free(buf);
        }

        temp1 = fread(&yesPtr[i], sizeof(int32_t), 1, fptr);
        if (temp1 != 1) {
            free(yesPtr);
            free(noPtr);
            free(nodePtr);
            fclose(fptr);
            return 0;
        }

        temp1 = fread(&noPtr[i],  sizeof(int32_t), 1, fptr);  
        if (temp1 != 1) {
            free(yesPtr);
            free(noPtr);
            free(nodePtr);
            fclose(fptr);
            return 0;
        }
    }

    for (i = 0; i < nodeCount; i++) {
        if (yesPtr[i] != -1) {
            nodePtr[i]->yes = nodePtr[yesPtr[i]];
        }
        if (noPtr[i] != -1) {
            nodePtr[i]->no = nodePtr[noPtr[i]];
        }
    }

    free_tree(g_root);
    g_root = nodePtr[0];
    free(nodePtr);
    free(yesPtr);
    free(noPtr);
    fclose(fptr);
    return 1;

}
