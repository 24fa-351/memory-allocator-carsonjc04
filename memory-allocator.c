#include "heap-memory-manager.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// Structure to manage memory blocks
typedef struct Block {
    size_t size;           // Size of the block
    int free;              // Is the block free?
    struct Block* next;    // Pointer to the next block
} Block;

#define BLOCK_SIZE sizeof(Block)
static Block* free_list = NULL;   // Head of the free block list
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Request more memory from the system
void* request_space(Block* last, size_t size) {
    Block* block = sbrk(0);
    void* request = sbrk(size + BLOCK_SIZE);
    if (request == (void*)-1) {
        return NULL; // sbrk failed
    }

    if (last) {
        last->next = block;
    }

    block->size = size;
    block->free = 0;
    block->next = NULL;
    return block;
}

// Find a free block of sufficient size
Block* find_free_block(Block** last, size_t size) {
    Block* current = free_list;
    while (current) {
        if (current->free && current->size >= size) {
            return current;
        }
        *last = current;
        current = current->next;
    }
    return NULL;
}

// Split a block if it's larger than the requested size
void split_block(Block* block, size_t size) {
    if (block->size > size + BLOCK_SIZE) {
        Block* new_block = (Block*)((char*)block + BLOCK_SIZE + size);
        new_block->size = block->size - size - BLOCK_SIZE;
        new_block->free = 1;
        new_block->next = block->next;

        block->size = size;
        block->next = new_block;
    }
}

// Coalesce adjacent free blocks
void coalesce_free_blocks() {
    Block* current = free_list;
    while (current && current->next) {
        if (current->free && current->next->free) {
            current->size += BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

// Allocate memory
void* my_malloc(size_t size) {
    pthread_mutex_lock(&lock);

    Block* block;
    if (size <= 0) {
        pthread_mutex_unlock(&lock);
        return NULL;
    }

    if (!free_list) {
        // No free list, request initial space
        block = request_space(NULL, size);
        if (!block) {
            pthread_mutex_unlock(&lock);
            return NULL;
        }
        free_list = block;
    } else {
        Block* last = free_list;
        block = find_free_block(&last, size);
        if (!block) {
            // No fitting block, request more space
            block = request_space(last, size);
            if (!block) {
                pthread_mutex_unlock(&lock);
                return NULL;
            }
        } else {
            // Use the found block
            block->free = 0;
            split_block(block, size);
        }
    }

    pthread_mutex_unlock(&lock);
    return (block + 1);
}

// Free allocated memory
void my_free(void* ptr) {
    if (!ptr) return;

    pthread_mutex_lock(&lock);

    Block* block = (Block*)ptr - 1;
    block->free = 1;

    coalesce_free_blocks();

    pthread_mutex_unlock(&lock);
}

// Reallocate memory
void* my_realloc(void* ptr, size_t size) {
    if (!ptr) {
        return my_malloc(size);
    }

    Block* block = (Block*)ptr - 1;
    if (block->size >= size) {
        return ptr;
    }

    void* new_ptr = my_malloc(size);
    if (!new_ptr) {
        return NULL;
    }

    memcpy(new_ptr, ptr, block->size);
    my_free(ptr);

    return new_ptr;
}
