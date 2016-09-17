
#include <kernel/screen.h>
#include <kernel/serial.h>

void main() {

	init_serial();
	clear_screen();

	printk("Hello, World!\n");
}

