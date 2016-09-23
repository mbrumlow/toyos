
#include <string.h>
#include <print.h>
#include <screen.h>
#include <serial.h>

void main(unsigned int top) {

	init_serial();
	clear_screen();

	kprintf("TOP: %d\n", top);

	while(1);
}

