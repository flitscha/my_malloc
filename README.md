# my_malloc

This program demonstrates a basic memory management simulation using C. The memory is represented as an integer array, initialized using create_memory(). Initially, the entire memory is a single free block.
When my_malloc() is called, the memory block is split into two parts: one part becomes occupied (allocated) and the other remains free. Each memory block is aware of its next and previous blocks, forming a doubly linked list structure.

## Compilation

if you have 'gcc' and 'make' installed, you can simply run the following command:

```bash
make

To run the program, simply execute this command:
```bash
./my_malloc
