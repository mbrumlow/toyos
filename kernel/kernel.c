
#include <kernel/string.h>
#include <kernel/print.h>
#include <kernel/screen.h>
#include <kernel/serial.h>

void main() {

	init_serial();
	clear_screen();

	kprintf("Hello, World!\n");

	while(1);
}

