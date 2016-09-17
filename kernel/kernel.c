
#include <kernel/string.h>
#include <kernel/print.h>
#include <kernel/screen.h>
#include <kernel/serial.h>

void main(unsigned int kstart, unsigned int kend) {

	init_serial();
	clear_screen();

	kprintf("kstart: 0x%08x, kend: 0x%08x, size: %d\n", kstart, kend, kend - kstart);

	while(1);
}

