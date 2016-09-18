
#include <kernel/string.h>
#include <kernel/print.h>
#include <kernel/screen.h>
#include <kernel/serial.h>

#include <kernel/page.h>

void main(void *start, void *end, void *page_directory ) {

	init_serial();
	clear_screen();

	kprintf("start: 0x%p, end: 0x%p\n", start, end);


//	init_paging(page_directory);

	while(1);
}

