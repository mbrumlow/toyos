
#include <kernel.h>
#include <page.h>
#include <memory.h>

static unsigned int *page_directory = 0;

void *get_physaddr(void *virtaddr);
void map_page(void *physaddr, void *virtaddr, unsigned int flags);

void init_paging(void *start, void *end) {

	// TODO

	// init_memory(end);

	/*
	asm volatile("mov %%eax, %%cr3": :"a"(page_directory));
	asm volatile("mov %cr0, %eax");
	asm volatile("or $0x80000000, %eax");
	asm volatile("mov %eax, %cr0");
	asm volatile("jmp +0x3D09");
	*/

}

void *get_physaddr(void *virtaddr) {

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

void map_page(void *physaddr, void *virtaddr, unsigned int flags) {

	unsigned int pdindex = (unsigned int)virtaddr >> 22;
	unsigned int ptindex = (unsigned int)virtaddr >> 12 & 0x03FF;

	unsigned int *pd = (unsigned int *) (page_directory + pdindex);
	unsigned int *pt = ((unsigned int *) (*pd & 0xFFFFF000)) + ptindex;

	*pt = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01;
}




