// ECE 312 - Memory Pool Allocator Lab
// Name: Ronit Verma
// EID:  RV24923

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "mempool.h"

/* Global pool instance */
MemoryPool g_pool = {0};

/* ============================================================================
 * HELPER FUNCTIONS (PROVIDED - DO NOT MODIFY)
 * ============================================================================ */

/*
 * Check if a pointer is within the pool's address range.
 */
static inline int ptr_in_pool(void* p) {
    return g_pool.pool_start &&
           (char*)p >= (char*)g_pool.pool_start &&
           (char*)p <  (char*)g_pool.pool_start + g_pool.total_size;
}

/*
 * Insert a block into the master list (all_list) after 'pos'.
 * If pos is NULL, insert at head.
 */
static void all_list_insert_after(MemoryBlock* pos, MemoryBlock* node) {
    if (!pos) {
        node->all_prev = NULL;
        node->all_next = g_pool.all_list;
        if (g_pool.all_list) g_pool.all_list->all_prev = node;
        g_pool.all_list = node;
        return;
    }
    node->all_prev = pos;
    node->all_next = pos->all_next;
    if (pos->all_next) pos->all_next->all_prev = node;
    pos->all_next = node;
}

/*
 * Remove a block from the master list (all_list).
 */
static void all_list_remove(MemoryBlock* node) {
    if (node->all_prev) node->all_prev->all_next = node->all_next;
    else                g_pool.all_list = node->all_next;
    if (node->all_next) node->all_next->all_prev = node->all_prev;
    node->all_prev = node->all_next = NULL;
}

/*
 * Find the block header for a given data pointer by scanning all_list.
 */
static MemoryBlock* find_block_by_ptr(void* ptr) {
    for (MemoryBlock* b = g_pool.all_list; b; b = b->all_next) {
        if (b->data == ptr) return b;
    }
    return NULL;
}

/* ============================================================================
 * STUDENT FUNCTIONS - IMPLEMENT THESE
 * ============================================================================ */

/*
 * pool_init: Initialize the memory pool
 * 
 * This function must:
 *   1. Check if pool is already initialized 
 *   2. Allocate memoryfor the pool 
 *   3. Create ONE initial free block that spans the entire pool:
 *   4. Initialize all g_pool fields:
 *   5. Add the initial block to all_list 
 * 
 * Returns: 0 on success, -1 on failure
 */
int pool_init(void) {
    // TODO: Implement this function

	if (g_pool.pool_start != NULL) {
		return -1;
	}

	int8_t* temp1 = (int8_t*) malloc(4096);

	if (temp1 == NULL) {
		return -1;
	}

	MemoryBlock* Block = (MemoryBlock*) malloc(sizeof(MemoryBlock));

	if (Block == NULL) {
		free(temp1);
		return -1;
	}

	// Initialize blocker_header fields

	Block->data = temp1;
	Block->size = 4096;
	Block->is_free = 1;
	Block->prev = NULL;
	Block->next = NULL;
	Block->all_prev = NULL;
	Block->all_next = NULL;

	// Initalize gpool fields

	g_pool.pool_start = temp1;
	g_pool.total_size = 4096;
	g_pool.free_size = 4096;
	g_pool.total_blocks = 1;
	g_pool.free_blocks = 1;
	g_pool.free_list = Block;
	g_pool.all_list = Block;
	return 0;
}

/*
 * pool_cleanup: Free all memory associated with the pool
 * 
 * This function must:
 *   1. Check pool_start 
 *   2. Free all block headers 
 *   3. Free the pool memory itself 
 *   4. Zero out g_pool 
 * 
 * HINT: Save the next pointer before freeing each node!
 */
void pool_cleanup(void) {
    // TODO: Implement this function

	if (g_pool.pool_start == NULL) {
		return;
	}

	MemoryBlock* Block = g_pool.all_list;

	while (Block != NULL) {
		MemoryBlock* temp = Block->all_next;
		free(Block);
		Block = temp;
	}

	free(g_pool.pool_start);
	memset(&g_pool, 0, sizeof(g_pool));
	return;
}

/*
 * find_free_block: Find a free block of at least 'size' bytes
 * 
 * This function must:
 *   1. Traverse the free_list from head to tail
 *   2. Return the FIRST block where that will satisfy the request 
 *   3. Return NULL if no suitable block is found
 * 
 * This implements the "first-fit" allocation strategy.
 * Do NOT remove the block from the free list here.
 */
MemoryBlock* find_free_block(size_t size) {
    // TODO: Implement this function

	if (size > g_pool.free_size || size == 0) {
		return NULL;
	}

	MemoryBlock* temp = g_pool.free_list;

	while (temp != NULL) {

		if (temp->size >= size) {
			return temp;
		} else {
			temp = temp->next;
		}
	}

	return NULL;
}

/*
 * add_to_free_list: Insert a block into the free list in address order
 * 
 * This function must:
 *   1. Mark the block as free
 *   2. Increment g_pool.free_blocks
 *   3. Find the correct position in free_list 
 *   4. Insert the block at that position.
 */
void add_to_free_list(MemoryBlock* block) {
    // TODO: Implement this function

	if (block == NULL) {
		return;
	}

	MemoryBlock* prev = NULL;
	block->next = NULL;
	block->prev = NULL;

	if (g_pool.free_list == NULL) {		// Insert at Head
		g_pool.free_list = block;
		g_pool.free_blocks++;
		block->is_free = 1;
		return;
	}

	MemoryBlock* curr = g_pool.free_list;

	while (curr != NULL && curr->data < block->data) {
		prev = curr;
		curr = curr->next;
	}

	if (curr == NULL) {		// Insert at Tail
		block->prev = prev;
		block->next = NULL;
		prev->next = block;
	} else {
		if (curr->prev == NULL) {		// Insert at Head
			block->prev = NULL;
			block->next = curr;
			curr->prev = block;
			g_pool.free_list = block;
		} else {		// Insert in Middle
			block->prev = prev;
			block->next = curr;
			curr->prev = block;
			prev->next = block;
		}
	}
	g_pool.free_blocks++;
	block->is_free = 1;
	return;
}

/*
 * remove_from_free_list: Remove a block from the free list
 * 
 * This function must:
 *   1. Update the appropriate pointers 
 *   2. Update  pointer(s) in the 'next block'
 *   3. Clear the block's pointers
 *   4. Decrement g_pool.free_blocks
 * 
 * Do NOT change is_free here - that's done by the caller.
 */
void remove_from_free_list(MemoryBlock* block) {
    // TODO: Implement this function

	if (block == NULL || g_pool.free_list == NULL ) {
		return;
	}

	MemoryBlock* prev = block->prev;
	MemoryBlock* next = block->next;

	// Remove from Head
	if (prev == NULL && next != NULL) {
		next->prev = NULL;
		g_pool.free_list = next;
	}

	// Only one Node
	if (prev == NULL && next == NULL) {
		g_pool.free_list = NULL;
	}

	// Remove from Tail
	if (prev != NULL && next == NULL) {
		prev->next = NULL;
	}

	// Remove from Middle
	if (prev != NULL && next != NULL) {
		prev->next = block->next;
		next->prev = block->prev;
	}

	block->next = NULL;
	block->prev = NULL;
	g_pool.free_blocks--;
	return;
}




/*
 * split_block: Split a block into allocated portion and free remainder
 * 
 * This function must:
 *   1. Check if block->size >= size (don't forget about alignment) 
 *   2. Allocate a NEW MemoryBlock header for the remainder (if any)
 *   3. Set up the new block.
 *   4. Add new block to all_list 
 *   5. Add new block to free_list 
 *   6. Update block->size = size
 *   7. Increment g_pool.total_blocks
 * 
 * IMPORTANT: The new block's header is allocated separately (on the heap),
 *            NOT carved from the pool. Only payload goes in the pool.
 */
void split_block(MemoryBlock* block, size_t size) {
    // TODO: Implement this function

	if (block == NULL) {
		return;
	}

	if (block->size < (size + ALIGNMENT)) {
		return;
	}

	size_t leftover = block->size - size;

	MemoryBlock* newBlock = (MemoryBlock*) malloc(sizeof(MemoryBlock));

	if (newBlock == NULL) {
		return;
	}

	newBlock->next = NULL;
	newBlock->prev = NULL;

	newBlock->size = leftover;
	newBlock->is_free = 1;
	newBlock->data = (char*) block->data + size;
	all_list_insert_after(block, newBlock);
	add_to_free_list(newBlock);
	block->size = size;
	g_pool.total_blocks++;
	return;

}

/*
 * coalesce_blocks: Merge adjacent free blocks
 * 
 * This function must:
 *   1. Walk through free_list checking each pair of adjacent blocks
 *   2. When merging cur and nxt:
 *      - Size is the sum of the two blocks 
 *      - Remove nxt from free_list
 *      - Remove nxt from all_list 
 *      - Release the header
 *      - Decrement g_pool.total_blocks
 *      - Catch chains of 3+ blocks
 *   3. If not adjacent, advance to next block
 * 
 * HINT: After merging, don't move cur forward - there might be another
 *       adjacent block to merge.
 */
void coalesce_blocks(void) {
    // TODO: Implement this function
    
	MemoryBlock* curr = g_pool.free_list;

	if (curr == NULL) {
		return;
	}

	MemoryBlock* nxt = curr->next;

	while (curr != NULL && nxt != NULL) {
		if ( (char*) curr->data + curr->size == (char*) nxt->data) {
			curr->size += nxt->size;
			remove_from_free_list(nxt);
			all_list_remove(nxt);
			free(nxt);
			g_pool.total_blocks--;
			nxt = curr->next;
		} else {
			curr = curr->next;
			if (curr == NULL) {
				return;
			} else {
				nxt = curr->next;
			}
		}
	}
	return;
}

/*
 * pool_malloc: Allocate memory from the pool
 * 
 * This function must:
 *   1. Check size 
 *   2. Account for aligntment
 *   3. Find a suitable block using find_free_block()
 *   4. If no block found, return NULL
 *   5. Remove the block from free_list
 *   6. Split the block if there's leftover space
 *   7. Mark the block as allocated (is_free = 0)
 *   8. Subtract the allocated size from g_pool.free_size
 *   9. Return block->data (the payload pointer)
 */
void* pool_malloc(size_t size) {
    // TODO: Implement this function

	if (size == 0) {
		return NULL;
	}

	size = ALIGN(size);

	if (size > g_pool.free_size) {
		return NULL;
	}

	MemoryBlock* ptr = find_free_block(size);

	if (ptr == NULL) {
		return NULL;
	}

	remove_from_free_list(ptr);
	split_block(ptr, size);
	ptr->is_free = 0;
	g_pool.free_size -= ptr->size;

	return ptr->data;

}

/*
 * pool_free: Free a previously allocated block
 * 
 * This function must:
 *   1. Return immediately if ptr is NULL
 *   2. Check if ptr is in the pool using 
 *   3. Find the block header 
 *   4. Check for double-free (is_free already 1); print error if so
 *   5. Add block->size back to g_pool.free_size
 *   6. Add the block back to free_list 
 *   7. Coalesce adjacent free blocks 
 */
void pool_free(void* ptr) {
    // TODO: Implement this function
    
	if (ptr == NULL) {
		return;
	}

	uint8_t x = ptr_in_pool(ptr);
	
	if (x != 1) {
		return;
	}

	MemoryBlock* block = find_block_by_ptr(ptr);

	if (block == NULL) {
		return;
	}

	if (block->is_free == 1) {
		printf("Error\n");
		return;
	}

	g_pool.free_size += block->size;
	add_to_free_list(block);
	coalesce_blocks();
	return;
}

/* ============================================================================
 * PROVIDED FUNCTION - DO NOT MODIFY
 * ============================================================================ */

/*
 * pool_stats: Print memory pool statistics
 */
void pool_stats(void) {
    size_t largest_free = 0, free_count = 0, free_bytes = 0;
    for (MemoryBlock* cur = g_pool.free_list; cur; cur = cur->next) {
        if (cur->is_free) {
            free_count++;
            free_bytes += cur->size;
            if (cur->size > largest_free) largest_free = cur->size;
        }
    }
    
    if (g_pool.free_size == 0 && free_bytes) g_pool.free_size = free_bytes;

    double frag = 0.0;
    if (g_pool.free_size > 0) {
        frag = 1.0 - (double)largest_free / (double)g_pool.free_size;
        if (frag < 0.0) frag = 0.0;
        if (frag > 1.0) frag = 1.0;
    }

    printf("=== Memory Pool Statistics ===\n");
    printf("Total pool size : %zu bytes\n", g_pool.total_size);
    printf("Free memory     : %zu bytes\n", g_pool.free_size);
    printf("Total blocks    : %d\n", g_pool.total_blocks);
    printf("Free blocks     : %d (scanned=%zu)\n", g_pool.free_blocks, free_count);
    printf("Largest free    : %zu bytes\n", largest_free);
    printf("Fragmentation   : %.1f%%\n", frag * 100.0);
    printf("==============================\n");
}
