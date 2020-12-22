#include "./mem.h"

static struct mem* get_memory_chunk(struct mem* ptr, struct mem* chunk);
static struct mem* allocate_new_memory(struct mem* ptr, size_t query);
static struct mem* get_last_chunk(struct mem* ptr);
static struct mem* find_free_chunk(size_t query, struct mem* prt);
static size_t round_memory(size_t n);
static void align_heap();

void* heap_init(size_t initial_size) {
	size_t size = round_memory(initial_size);

	struct mem* head = mmap(HEAP_START, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	head -> next = NULL;
	head -> capacity = size - sizeof(struct mem);
	head -> is_free = true;

	return (void*) (head + 1);
}

void* _malloc(size_t query) {
	struct mem* ptr = find_free_chunk(query, HEAP_START);
	struct mem* new_ptr;

	if(ptr) {
		new_ptr = (struct mem*) ((char*)ptr + sizeof(struct mem) + query);
		new_ptr -> capacity = ptr -> capacity - query - sizeof(struct mem);
		new_ptr -> is_free = true;

		ptr -> next = new_ptr;
		ptr -> capacity = query;
		ptr -> is_free = false;
		return (char*)ptr +sizeof(struct mem);
	} else {
		ptr = get_last_chunk(HEAP_START);
		new_ptr = allocate_new_memory(ptr, query);
		return _malloc(query);
	}
}

static void align_heap() {
	struct mem* current = (struct mem*) HEAP_START;
	while(current -> next) {
			if(current -> is_free && (current -> next) -> is_free) {
				current -> capacity += (current -> next) -> capacity;
				current -> next = (current -> next) -> next;
			} else {
				current = current -> next;
			}
		}
}

void _free(void* mem) {
	struct mem* f_chunk = get_memory_chunk(HEAP_START, (struct mem*)((char*)mem - sizeof(struct mem)));
	if(f_chunk) {
		f_chunk -> is_free = true;
		align_heap();
	}
}

static size_t round_memory(size_t n) {
	if(n % BLOCK_SIZE) {
		return n + (BLOCK_SIZE - n % BLOCK_SIZE);
	} else {
		return n;
	}
}

static struct mem* find_free_chunk(size_t query, struct mem* ptr) {
	while(ptr) {
		if(ptr -> is_free && ptr -> capacity >= query + sizeof(struct mem))
			return ptr;
		ptr = ptr -> next;
	}

	return NULL;
}

static struct mem* get_last_chunk(struct mem* ptr) {
	while(ptr -> next) {
		ptr = ptr -> next;
	}
	return ptr;
}

static struct mem* allocate_new_memory(struct mem* ptr, size_t query) {
	char* addr = (char*)(ptr) + ptr -> capacity + sizeof(struct mem);
	size_t size = round_memory(query + sizeof(struct mem));

	void* new_ptr = mmap((void*) addr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	
	if(new_ptr == MAP_FAILED)
		new_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(new_ptr == MAP_FAILED)
		return NULL;
	if(new_ptr == addr) {
			ptr -> capacity += size;
		return ptr;
	} else {
		ptr -> next = (struct mem*) new_ptr;

		(ptr -> next) -> next = NULL;
		(ptr -> next) -> capacity = size - sizeof(struct mem);
		(ptr -> next) -> is_free = true;

		return (struct mem*) new_ptr;
	}

}

static struct mem* get_memory_chunk(struct mem* ptr, struct mem* chunk) {
	while(ptr != NULL && ptr != chunk) {
		ptr = ptr -> next;
	}
	return ptr;
}