
#include <memory.h>
#include <page.h>
#include <print.h>

#define MAX_ALLOC 32

extern void *end;
static void *heap_top = (void *) (&end + 0x1000);

static void *slab[32];

void init_heap() {

	for(int i=0; i < MAX_ALLOC; i++) {
		slab[i] = 0;
	}

}

void *kmalloc(int size) {

	void *ptr, *heap;
	int i, idx = 0;
	int sz = 4;

	for(i = 1; i < MAX_ALLOC; i++) {

		if(sz < size) {
			sz = sz << 1;
			continue;
		}

		idx = i;
		size = sz;

		for(; i < MAX_ALLOC; i++) {

			if(slab[i]) {

				if(size == sz) {
					ptr = slab[i];
					slab[i] = (void *) *((unsigned int *)ptr);
					ptr += sizeof(unsigned int) + sizeof(unsigned char);
					return ptr;
				} else {
					// TODO: consider breaking up larger entries.
				}

			}
		}

		break;
	}

	if(!mapped(heap_top)) {
		void *frame = alloc_frame();
		kmap(frame, heap_top);
	}

	heap = (void *) ((unsigned int) heap_top & 0xFFFFF000) + 0x1000;
	while(size + sizeof(unsigned int) + sizeof(unsigned char) > heap - heap_top) {
		void *frame = alloc_frame();
		kmap(frame, heap);
		heap += 0x1000;
	}

	unsigned int *next = (unsigned int *) heap_top;
	*next = 0;
	heap_top += sizeof(unsigned int);

	unsigned char *index = (unsigned char *) heap_top;
	*index = (unsigned char) idx;
	heap_top += sizeof(unsigned char);

	ptr = heap_top;
	heap_top += size;
	return ptr;

}

void kfree(void *ptr) {

	int i;
	unsigned int *p, *pp;
	unsigned int *next;
	unsigned char *index;

	ptr -= sizeof(unsigned char);
	index = ptr;

	ptr -= sizeof(unsigned int);
	next = ptr;

	i = (int) *index;

	if(slab[i]) {

		// Insert sorted for now. This will help when we do the clean up later.

		p = (unsigned int *) slab[i];
		pp = p;
		while(p) {
			if((unsigned int *)ptr < p)
				break;

			pp = p;
			p = (unsigned int *) *p;
		}

		*pp = (unsigned int) ptr;
		*((unsigned int *)ptr) = (unsigned int) p;

	} else {
		slab[i] = ptr;
	}

	// TODO: clean up slab.
}

