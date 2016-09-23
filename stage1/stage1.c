
unsigned int map_range(unsigned int top,
		unsigned int *page_directory,
		unsigned int virtaddr,
		unsigned int physaddr,
		unsigned int count);

unsigned int main(unsigned int kernel_sectors)  {

	unsigned int top = 0x8000 + (kernel_sectors * 512);
	unsigned int *page_directory = (unsigned int *) top;

	top += 4096;

	for(int i = 0; i < 1024; i++)
		page_directory[i] = 0x00000000;

	top = map_range(top, page_directory, 0x00000000, 0x00000000, 256);
	top = map_range(top, page_directory, 0x4000000, 0x8000, (kernel_sectors*512) / 4096);

	asm volatile("mov %%eax, %%cr3": :"a"(page_directory));
	asm volatile("mov %cr0, %eax");
	asm volatile("or $0x80000000, %eax");
	asm volatile("mov %eax, %cr0");

	return top;
}

unsigned int map_range(unsigned int top,
		unsigned int *page_directory,
		unsigned int virtaddr,
		unsigned int physaddr,
		unsigned int count)  {

	for(int i = 0; i < count; i++) {

		unsigned int pdindex = (unsigned int)virtaddr >> 22;
		unsigned int ptindex = (unsigned int)virtaddr >> 12 & 0x03FF;

		unsigned int *pd = (unsigned int *) (page_directory + pdindex);

		if(!(*pd & 0x00000001)) {
			*pd = top | 0x00000001;
			top += 4096;
		}

		unsigned int *pt = ((unsigned int *) (*pd & 0xFFFFF000)) + ptindex;
		*pt = ((unsigned long)physaddr) | 0x01;

		virtaddr += 4096;
		physaddr += 4096;
	}

	return top;
}


