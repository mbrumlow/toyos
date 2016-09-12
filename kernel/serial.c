
#include <kernel/serial.h>
#include <kernel/port.h>

#define REG_SERIAL_PORT 0x3f8

int serial_enabled = 0;

void init_serial() {

	// TODO -- make this configurable.

	port_byte_out(REG_SERIAL_PORT+1, 0x00);
	port_byte_out(REG_SERIAL_PORT+3, 0x80);
	port_byte_out(REG_SERIAL_PORT+0, 0x03);
	port_byte_out(REG_SERIAL_PORT+1, 0x00);
	port_byte_out(REG_SERIAL_PORT+3, 0x03);
	port_byte_out(REG_SERIAL_PORT+2, 0xc7);
	port_byte_out(REG_SERIAL_PORT+4, 0x0b);

	serial_enabled = 1;
}

int is_transmit_empty() {
	return port_byte_in(REG_SERIAL_PORT+5) & 0x20;
}

void print_char_serial(char character) {

	if(!serial_enabled)
		return;

	while(is_transmit_empty() == 0);
	port_byte_out(REG_SERIAL_PORT, character);
}

