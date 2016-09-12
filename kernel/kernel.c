
#include <kernel/screen.h>
#include <kernel/serial.h>

void main() {

	clear_screen();
	init_serial();

	printk("Hello, World!\n");
}

