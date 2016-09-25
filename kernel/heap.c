
#include <memory.h>
#include <page.h>
#include <print.h>

#define MAX_ALLOC 22

extern void *end;
static void *heap_top = (void *) (&end + 0x1000);
static void *prev = 0;

typedef struct __attribute__((__packed__)) {
	void *next;
	void *prev;
	unsigned char size;
} alloc_header;

static alloc_header *slab[32];

void init_heap() {
	for(int i=0; i < MAX_ALLOC; i++) {
		slab[i] = 0;
	}
}

void *kmalloc(int size) {

	alloc_header *ah;
	void *ptr, *heap;
	int i, idx = 0;
	int sz;

	for(i = 1; i < MAX_ALLOC; i++) {

		sz = 4 << i-1;

		if(sz < size)
			continue;

		idx = i;
		size = sz;

		for(; i < MAX_ALLOC; i++) {

			if(slab[i-1]) {

				if(size == sz) {
					ah = slab[i-1];
					slab[i-1] = ah->next;
					ptr = ah + sizeof(alloc_header);
					ah->size = ah->size & ~0x80;
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
		kprintf("Removing from slab head!\n");
		slab[idx] = ah->next;
		return;
	}

	n = slab[idx]->next;
	p = slab[idx];

	while(n) {
		if(n == ah) {
			kprintf("Removing from slab!\n");
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
	unsigned int idx = ((unsigned int) ah->size & 0x0000007F)-1;
	unsigned int size = 4 << idx;
	unsigned int start = (unsigned int) ptr - sizeof(alloc_header);
	unsigned int end = (((unsigned int )ptr + size) & 0xFFFFF000);

	while(ptr + size == heap_top) {

		heap_top = (void *) start;
		prev = ah->prev;

		if( start == (start & 0xFFFFF000) ) {
			kunmap((void *) start);
			put_frame((void *) start);
		} else {
			start = (start & 0xFFFFF000);
		}

		start += 0x1000;
		while(start <= end) {
			kunmap((void *) start);
			put_frame((void *) start);
			start += 0x1000;
		}

		// If prev is 0 / NULL then no further processing is needed.
		if(!prev)
			return;

		// prep everything for top of loop based off prev.
		ptr = prev;
		ah = ptr - sizeof(alloc_header);
		start = (unsigned int)ptr - sizeof(alloc_header);
		end = (((unsigned int )ptr + size) & 0xFFFFF000);

		if(ah->size & 0x80) {
			remove_slab(idx, ah);
			continue;
		}

		return;
	}

	ah->size = ah->size | 0x80;
	add_slab(idx, ah);
}

