#include <stdio.h>
#include <stdlib.h>

#define NUM_INFORMATIONS 4
enum block_information {
    BLOCK_SIZE, // including the 4 block-informations
    BLOCK_PREV,
    BLOCK_NEXT,
    BLOCK_IS_EMPTY
};

int* create_memory(int n) {
    // allocating memory
    int* memory = (int*)malloc(sizeof(int)*n);
    if (memory == NULL) {
        fprintf(stderr, "error while creating the memory\n");
        return NULL;
    }

    // creating one memory block, that is free
    memory[BLOCK_SIZE] = n;
    memory[BLOCK_PREV] = -1;
    memory[BLOCK_NEXT] = -1;
    memory[BLOCK_IS_EMPTY] = 1;
    return memory;
}



int my_malloc(int* p, int n) {
    // search the first block with enough memory
    int current_index = 0;
    int current_size = p[current_index+BLOCK_SIZE];
    int is_empty = p[current_index+BLOCK_IS_EMPTY];
    while (is_empty == 0 || current_size < n+NUM_INFORMATIONS) {
        current_index = p[current_index+BLOCK_NEXT];
        if (current_index < 0) {
            fprintf(stderr, "error while allocating memory\n");
            return -1;
        }
        current_size = p[current_index+BLOCK_SIZE];
        is_empty = p[current_index+BLOCK_IS_EMPTY];
    }

    // if the block has already the correct size, we dont need to split the block
    if (p[current_index+BLOCK_SIZE] == n + NUM_INFORMATIONS) {
        p[current_index+BLOCK_IS_EMPTY] = 0;
        return current_index;
    }

    // otherwise split the block into 2 blocks
    int size_before = p[current_index+BLOCK_SIZE];
    int next_index = p[current_index+BLOCK_NEXT];
    int new_block_index = current_index + n + NUM_INFORMATIONS;

        // update the informations of the allocated block
    p[current_index+BLOCK_NEXT] = new_block_index;
    p[current_index+BLOCK_IS_EMPTY] = 0;
    p[current_index+BLOCK_SIZE] = n + NUM_INFORMATIONS;

        // informations of the new empty block
    p[new_block_index+BLOCK_NEXT] = next_index;
    p[new_block_index+BLOCK_SIZE] = size_before - n - NUM_INFORMATIONS;
    p[new_block_index+BLOCK_IS_EMPTY] = 1;
    p[new_block_index+BLOCK_PREV] = current_index;

    return current_index;
}


void my_free(int* memory, int index) {
    memory[index+BLOCK_IS_EMPTY] = 1;

    // we want to merge the block with the neighbors, if they are free
    // these variables describe the free block. They are changing, if there is a merge operation
    int block_index = index;
    int block_size = memory[index+BLOCK_SIZE];
    int next_block_index = memory[index+BLOCK_NEXT];
    int prev_block_index = memory[index+BLOCK_PREV];

    // if prev_block is empty, merge the two blocks
    if (prev_block_index >= 0 && memory[prev_block_index+BLOCK_IS_EMPTY]) {
        block_index = prev_block_index;
        block_size += memory[prev_block_index+BLOCK_SIZE];
        prev_block_index = memory[prev_block_index+BLOCK_PREV];

        // update the block
        memory[block_index+BLOCK_NEXT] = next_block_index;
        memory[block_index+BLOCK_SIZE] = block_size;
        
        // update the next block
        if (next_block_index >= 0) {
            memory[next_block_index+BLOCK_PREV] = block_index;
        }
    }

    // if next_block is empty, merge it with its previous block
    if (next_block_index >= 0 && memory[next_block_index+BLOCK_IS_EMPTY]) {
        block_size += memory[next_block_index+BLOCK_SIZE];
        next_block_index = memory[next_block_index+BLOCK_NEXT];

        // update the block
        memory[block_index+BLOCK_NEXT] = next_block_index;
        memory[block_index+BLOCK_SIZE] = block_size;

        // update the next block
        if (next_block_index >= 0) {
            memory[next_block_index+BLOCK_PREV] = block_index;
        }
    }
}


void print_memory(int* memory) {
    int current_index = 0;
    int block_counter = 1;
    while (current_index != -1) {
        printf("block_%d\n", block_counter);
        printf("index: %d\n", current_index);
        printf("size: %d\n", memory[current_index+BLOCK_SIZE]);
        printf("state: %s\n\n", memory[current_index+BLOCK_IS_EMPTY] ? "empty" : "full");
        current_index = memory[current_index+BLOCK_NEXT];
        block_counter++;
    }
}

void print_memory_visual(int* memory, int memory_size) {
    const int image_width = 100;

    double chars_per_unit = (double)image_width / (double)memory_size;
    int current_index = 0;

    while (current_index != -1) {
        int block_size = memory[current_index+BLOCK_SIZE];
        int is_empty = memory[current_index+BLOCK_IS_EMPTY];

    	printf("|");
        for (double num_chars = 0; num_chars < block_size*chars_per_unit-1; num_chars++) {
            if (is_empty) {
                printf(".");
            }
            else {
                printf("#");
            }
        }
        current_index = memory[current_index+BLOCK_NEXT];
    }
    printf("|\n");
}


int main(void) {
    int* p = create_memory(200);
    print_memory_visual(p, 200);
    my_malloc(p, 50);
    print_memory_visual(p, 200);
    int a = my_malloc(p, 20);
    print_memory_visual(p, 200);
    int b = my_malloc(p, 50);
    print_memory_visual(p, 200);
    my_free(p, a);
    print_memory_visual(p, 200);
    my_malloc(p, 50);
    print_memory_visual(p, 200);
    my_free(p, b);
    print_memory_visual(p, 200);
    my_malloc(p, 50);
    //print_memory(p);
    print_memory_visual(p, 200);  

    return EXIT_SUCCESS;
}
