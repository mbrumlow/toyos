
#include <kernel.h>
#include <page.h>
#include <memory.h>
#include <print.h>

extern unsigned int end;

static unsigned int *_page_directory = 0;
static unsigned int _end = (unsigned int) &end;

void map_page(unsigned int *page_directory, void *physaddr, void *virtaddr, unsigned int flags);
void *get_physaddr(unsigned int *page_directory, void *virtaddr);

void init_paging(void *top) {

	asm volatile("mov %%cr3, %%eax": "=a"(_page_directory): );

	init_memory(top);

	// TODO
	// Rebuild page table using heap allocated pointers.
	// Move 1M to a new high location.

	/*
	asm volatile("mov %%eax, %%cr3": :"a"(page_directory));
	asm volatile("mov %cr0, %eax");
	asm volatile("or $0x80000000, %eax");
	asm volatile("mov %eax, %cr0");
	*/

}

void *get_physaddr(unsigned int *page_directory, void *virtaddr) {

	if(!page_directory)
		return 0;

	unsigned int pdindex = (unsigned int)virtaddr >> 22;
	unsigned int ptindex = (unsigned int)virtaddr >> 12 & 0x03FF;

	unsigned int *pd = (unsigned int *) (page_directory + pdindex);

	if(!(*pd & 0x00000001))
		return NULL;

	unsigned int *pt = ((unsigned int *) (*pd & 0xFFFFF000)) + ptindex;


	if(!(*pt & 0x00000001))
		return NULL;

	return (void *) ((*pt & ~0xFFF) + ((unsigned long)virtaddr & 0xFFF));
}

int mapped(void *virtaddr) {

	if(get_physaddr(_page_directory, virtaddr))
		return 1;

	return 0;
}

void kmap(void *physaddr, void *virtaddr) {
	map_page(_page_directory, physaddr, virtaddr, 0x02);
	return;
}


void map_page(unsigned int *page_directory, void *physaddr, void *virtaddr, unsigned int flags) {

	unsigned int pdindex = (unsigned int)virtaddr >> 22;
	unsigned int ptindex = (unsigned int)virtaddr >> 12 & 0x03FF;

	unsigned int *pd = (unsigned int *) (page_directory + pdindex);
	unsigned int *pt = ((unsigned int *) (*pd & 0xFFFFF000)) + ptindex;

	*pt = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01;
}

void map_range(unsigned int *page_directory, void *physaddr, void *virtaddr, unsigned int flags, int count) {

	for(int i = 0; i < count; i++) {
		map_page(page_directory, physaddr, virtaddr, flags);
		physaddr += 4096;
		virtaddr += 4096;
	}
}

