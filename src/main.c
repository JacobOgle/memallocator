#include "stdio.h"
#include "stdlib.h"

/* =========================================================

When we malloc mem to the heap we need to be able to free it.
In-order to do this we must know the size to free.
Also, heap memory is noot contiguious so we will
essentially create a linked list like structure
to track the block chunks of memory.

 ========================================================= */
typedef char ALIGN[16]; // align to 16 bytes

union header{
        struct header_t {
        size_t size;
        unsigned is_free;
        struct header_t* next;
    } s;
    ALIGN stub;
};
typedef union header header_t;

void* malloc(size_t size);

int main(){
    printf("Hey! I am just a placeholder in main");
    return EXIT_SUCCESS;
}

/* =========================================================
Allocates memory to the heap by incrementing the brk
pointer. This uses the syscall sbrk

Params:
    size_t size - the size of bytes to be allocated

Return:
    void* block - the pointer to the newly allocated memory
 ========================================================= */
void* malloc(size_t size){
    void* block;
    // calling sbrk(size) requests us to increment brk by the size in bytes
    // a + size allocates mem and a - size deallocates mem
    // sbrk is not the best choice here, but for this simple demo it will suffice
    // sbrk has threading issue`.
    block = sbrk(size);
    if (block == (void*)-1){
        return NULL;
    }
    return block;
}