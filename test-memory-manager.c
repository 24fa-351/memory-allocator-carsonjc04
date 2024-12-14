#include "heap-memory-manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function prototypes
void test_malloc_and_free();
void test_realloc();
void test_multiple_allocations();

// Main function
int main(int argc, char* argv[]) {
    if (argc == 3 && strcmp(argv[1], "-t") == 0) {
        int test_num = atoi(argv[2]);
        switch (test_num) {
            case 1:
                test_malloc_and_free();
                break;
            case 2:
                test_realloc();
                break;
            case 3:
                test_multiple_allocations();
                break;
            default:
                printf("Unknown test number: %d\n", test_num);
        }
    } else {
        printf("Running all tests...\n");
        test_malloc_and_free();
        test_realloc();
        test_multiple_allocations();
        printf("All tests completed.\n");
    }
    return 0;
}

// Function to test malloc and free
void test_malloc_and_free() {
    printf("Running test_malloc_and_free...\n");
    void* ptr = malloc(128);
    if (ptr == NULL) {
        printf("FAILED: malloc returned NULL\n");
        return;
    }
    memset(ptr, 0, 128);
    free(ptr);
    printf("PASSED: test_malloc_and_free\n");
}

// Function to test realloc
void test_realloc() {
    printf("Running test_realloc...\n");
    void* ptr = malloc(64);
    if (ptr == NULL) {
        printf("FAILED: malloc returned NULL\n");
        return;
    }
    memset(ptr, 0, 64);

    void* new_ptr = realloc(ptr, 128);
    if (new_ptr == NULL) {
        printf("FAILED: realloc returned NULL\n");
        free(ptr);
        return;
    }
    memset(new_ptr, 0, 128);

    free(new_ptr);
    printf("PASSED: test_realloc\n");
}

// Function to test multiple allocations
void test_multiple_allocations() {
    printf("Running test_multiple_allocations...\n");
    void* ptr1 = malloc(32);
    void* ptr2 = malloc(64);
    void* ptr3 = malloc(128);

    if (ptr1 == NULL || ptr2 == NULL || ptr3 == NULL) {
        printf("FAILED: malloc returned NULL\n");
        free(ptr1);
        free(ptr2);
        free(ptr3);
        return;
    }

    memset(ptr1, 0, 32);
    memset(ptr2, 0, 64);
    memset(ptr3, 0, 128);

    free(ptr1);
    free(ptr2);
    free(ptr3);
    printf("PASSED: test_multiple_allocations\n");
}
