
#include <kernel/string.h>
#include <kernel/print.h>
#include <kernel/screen.h>
#include <kernel/serial.h>

#include <kernel/memory.h>

void main(void *start, void *end, void *page_directory ) {

	init_serial();
	clear_screen();

	init_memory(end);

	kprintf("start: 0x%p, end: 0x%p\n", start, end);

	while(1);
}

