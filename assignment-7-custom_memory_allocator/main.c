#include <stdio.h>
#include "./mem.h"

int main(int argc, char** arv) {
	heap_init(1);
	puts("\n------ Before allocation ------\n");
	memalloc_debug_heap(stdout, HEAP_START);

	void*  chunk_1 = _malloc(1000);
	void*  chunk_2 = _malloc(1100);
	void*  chunk_3 = _malloc(1200);
	void*  chunk_4 = _malloc(1300);
	void*  chunk_5 = _malloc(1400);
	puts("\n------ After allocation ------\n");
	memalloc_debug_heap(stdout, HEAP_START);

	_free(chunk_3);
	puts("\n------ After freed chunk_3 ------\n");
	memalloc_debug_heap(stdout, HEAP_START);
	
	_free(chunk_5);
	puts("\n------ After freed chunk_5 ------\n");
	memalloc_debug_heap(stdout, HEAP_START);

	return 0;
}