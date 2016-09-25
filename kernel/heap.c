
#include <memory.h>
#include <page.h>
#include <print.h>

#define MAX_ALLOC 32

extern void *end;
static void *heap_top = (void *) (&end + 0x1000);
static void *prev = 0;

typedef struct __attribute__((__packed__)) {
	void *next;
	void *prev;
	unsigned char size;
} alloc_header;

static alloc_header *slab[MAX_ALLOC];

void init_heap() {
	for(int i=0; i < MAX_ALLOC; i++) {
		slab[i] = 0;
	}
}

unsigned int bsf(unsigned int n) {
	unsigned int result;
	__asm__("bsf %%dx, %%ax" : "=a" (result) : "d" (n));
	return result;
}

unsigned int nextPower2(unsigned int n)  {

	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n++;

	return n;
}


void *kmalloc(int size) {

	alloc_header *ah;
	void *ptr, *heap;
	int sz = nextPower2(size);
	int idx = bsf(sz);

	kprintf("kmalloc: size: %d\n", size);

	if(slab[idx]) {
		ah = slab[idx];
		slab[idx] = ah->next;
		ptr = ++ah;
		ah->size = ah->size & ~0x80;
		kprintf("kmalloc: found cache entry %p/%p\n", ah, ptr);
		return ptr;
	}

	if(!mapped(heap_top)) {
		void *frame = alloc_frame();
		kmap(frame, heap_top);
	}

	// allocate the pages required to satisfy this heap.
	heap = (void *) ((unsigned int) heap_top & 0xFFFFF000) + 0x1000;
	while(size + sizeof(ah) > heap - heap_top) {
		void *frame = alloc_frame();
		// TODO check for failure (alloc_frame() does not support this yet).
		kmap(frame, heap);
		heap += 0x1000;
	}

	ah = (alloc_header *) heap_top;

	ah->next = 0;
	ah->size = idx;
	ah->prev = prev;

	heap_top += sizeof(alloc_header);

	ptr = heap_top;
	prev = ptr;

	heap_top += size;
	return ptr;
}

void remove_slab(int idx, alloc_header *ah) {

	alloc_header *p, *n;

	if(!slab[idx]) {
		kprintf("WARNING: remove_slab with no slab entry!\n");
		return;
	}

	if(slab[idx] == ah) {
		slab[idx] = ah->next;
		return;
	}

	n = slab[idx]->next;
	p = slab[idx];

	while(n) {
		if(n == ah) {
			p->next = n->next;
			return;
		}

		p = n;
		n = n->next;
	}

	kprintf("WARNING: remove_slab but was not in slab!\n");

	return;
}

void add_slab(int idx, alloc_header *ah) {

	alloc_header *p, *n;

	if(!slab[idx]) {
		slab[idx] = ah;
		return;
	}

	if(ah < slab[idx])  {
		ah->next = slab[idx];
		slab[idx] = ah;
		return;
	}

	n = slab[idx]->next;
	p = slab[idx];

	while(n) {
		if(ah < n)  {
			break;
		}
		p = n;
		n = n->next;
	}

	p->next = ah;
	ah->next = n;
}


void kfree(void *ptr) {

	alloc_header *ah = ptr - sizeof(alloc_header);
	unsigned int idx = ((unsigned int)ah->size) & 0x0000007F;
	unsigned int size = 1 << idx;
	unsigned int start = ((unsigned int) ptr) - sizeof(alloc_header);
	unsigned int end = ((unsigned int )ptr + size) & 0xFFFFF000;

	kprintf("kfree: ptr: %p, size: %d => %p | heap_top: %p\n",
			ptr, size, ptr + size, heap_top);

	while(ptr + size == heap_top) {

		kprintf("kfree: recovering heap %p -> %p\n", heap_top, (void *) start);

		heap_top = (void *) start;
		prev = ah->prev;

		if( start == (start & 0xFFFFF000) ) {
			kprintf("kfree: recovering last page: %p\n", (void *) start);
			kunmap((void *) start);
			put_frame((void *) start);
		} else {
			start = (start & 0xFFFFF000);
		}

		start += 0x1000;
		while(start <= end) {
			kprintf("kfree: recovering another page: %p\n", (void *) start);
			kunmap((void *) start);
			put_frame((void *) start);
			start += 0x1000;
		}

		// If prev is 0 / NULL then no further processing is needed.
		if(!prev)
			return;

		kprintf("kfree: checking for more slabs to free @ %p\n", prev);

		// prep everything for top of loop based off prev.
		ptr = prev;
		ah = ptr - sizeof(alloc_header);
		start = (unsigned int)ptr - sizeof(alloc_header);
		end = (((unsigned int )ptr + size) & 0xFFFFF000);

		if(ah->size & 0x80) {
			kprintf("kfree: found slab to free @ %p\n", prev);
			remove_slab(idx, ah);
			continue;
		}

		kprintf("kfree: all done!\n");

		return;
	}

	kprintf("kfree: caching slab %p/%p, size: %d\n", ah,ptr, size);

	ah->size = ah->size | 0x80;
	add_slab(idx, ah);
}

