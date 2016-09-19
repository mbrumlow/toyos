
#include <kernel/kernel.h>

/*
void *get_physaddr(void *virtaddr) {

	if(!page_directory)
		return 0;

	unsigned int pdindex = (unsigned int)virtaddr >> 22;
	unsigned int ptindex = (unsigned int)virtaddr >> 12 & 0x03FF;

	unsigned int *pd = (unsigned int *) (page_directory + pdindex);

	if(!(*pd & 0x00000003))
		return NULL;

	unsigned int *pt = ((unsigned int *) (*pd & 0xFFFFF000)) + ptindex;

	if(!(*pt & 0x00000003))
		return NULL;

	return (void *) ((*pt & ~0xFFF) + ((unsigned long)virtaddr & 0xFFF));
}
*/
