
#include <string.h>
#include <print.h>
#include <screen.h>
#include <serial.h>
#include <page.h>
#include <heap.h>

void main(void *top) {

	init_serial();
	clear_screen();

	init_paging(top);
	init_heap();

	int *i = kmalloc(sizeof(int));
	if(!i) {
		kprintf("Failed to allocate int :(\n");
	} else {
		kprintf("Allocated int @ %p\n", i);
		*i = 123;
		kprintf("Stored 123 in *int @ %p -> %d\n", i, *i);
	}

	while(1);
}

