#include "stdio.h"
#include "stdlib.h"

void* malloc(size_t size);


int main(){
    printf("Hey! I am just a placeholder in main");
    return EXIT_SUCCESS;
}

/*
Allocates memory to the heap by incrementing the brk
pointer. This uses the syscall sbrk

Params:
    size_t size - the size of bytes to be allocated

Return:
    void* block - the pointer to the newly allocated memory
*/
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