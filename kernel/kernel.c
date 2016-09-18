
#include <kernel/string.h>
#include <kernel/print.h>
#include <kernel/screen.h>
#include <kernel/serial.h>

#include <kernel/page.h>

void main(void *kstart, void *kend, void *page_directory ) {

	init_serial();
	clear_screen();

	kprintf("kstart: 0x%p, kend: 0x%p, pd: 0x%p\n", kstart, kend, page_directory);

	init_paging(page_directory);

	while(1);
}

