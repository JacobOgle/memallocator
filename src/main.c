#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

/* ===========================================================================================

When we malloc mem to the heap we need to be able to free it.
In-order to do this we must know the size to free.
Also, heap memory is noot contiguious so we will
essentially create a linked list like structure
to track the block chunks of memory.

----------------------------
| Header | 16 Bytes Memory |
----------------------------

Allows header to point to next mem block allocated

 ========================================================= */
typedef char ALIGN[16]; // align to 16 bytes

union header
{ // Union is the largest size of its members
	struct header_t
	{
		size_t size;
		unsigned is_free;
		struct header_t *next;
	} s;
	ALIGN stub; // Allows the 16 byte alignment
};
typedef union header header_t;

// ===========================================================================================

// Global Lock ===============================================================================
pthread_mutex_t global_malloc_lock; // to modify memory you need to access and free the lock

// Forward Declarations ======================================================================
header_t *get_free_block(size_t size);
void *malloc(size_t size);

// Main ======================================================================================
int main()
{
	printf("Hey! I am just a placeholder in main");
	return EXIT_SUCCESS;
}

// Function Definitions ======================================================================

/* =========================================================
Allocates memory to the heap by incrementing the brk
pointer. This uses the syscall sbrk.

Note* sbrk is not thread safe and is only used in this
project for learning purposes

Params:
	size_t size - the size of bytes to be allocated

Return:
	void* block - the pointer to the newly allocated memory
 ========================================================= */
void *malloc(size_t size)
{

	size_t total_size;
	void *block;
	header_t *header;

	// Check if we are requesting 0 size
	if (!size)
		return NULL;

	pthread_mutex_lock(&global_malloc_lock);

	// Traverses linked list of mem allocs
	header = get_free_block(size);

	if (header)
	{
		header->s.is_free = 0;
		pthread_mutex_unlock(&global_malloc_lock);
		return (void *)(header + 1);
	}
	total_size = sizeof(header_t) + size;
	block = sbrk(total_size);
	if (block == (void *)-1)
	{
		pthread_mutex_unlock(&global_malloc_lock);
		return NULL;
	}
	header = block;
	header->s.size = size;
	header->s.is_free = 0;
	header->s.next = NULL;
	if (!head)
		head = header;
	if (tail)
		tail->s.next = header;
	tail = header;
	pthread_mutex_unlock(&global_malloc_lock);
	return (void *)(header + 1);
}

header_t *get_free_block(size_t size)
{
	header_t *curr = head;
	while (curr)
	{
		if (curr->s.is_free && curr->s.size >= size)
			return curr;
		curr = curr->s.next;
	}
	return NULL;
}