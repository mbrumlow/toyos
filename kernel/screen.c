
#include <kernel/screen.h>
#include <kernel/string.h>
#include <kernel/port.h>
#include <kernel/serial.h>

# define VIDEO_ADDRESS 0xb8000
# define MAX_ROWS 25
# define MAX_COLS 80

// Screen device I/O ports
# define REG_SCREEN_CTRL 0x3D4
# define REG_SCREEN_DATA 0x3D5

int get_cursor();
void set_cursor(int offset);
int get_screen_offset(int col, int row);
int handle_scrolling(int cursor_offset);

void print_char(char character, int col, int row, char attribute_byte) {

	unsigned char *video_memory = (unsigned char *) VIDEO_ADDRESS;

	if(!attribute_byte) {
		attribute_byte = WHITE_ON_BLACK;
	}

	int offset;
	if(col >= 0 && row >= 0) {
		offset = get_screen_offset(col, row);
	} else {
		offset = get_cursor();
	}

	if(character == '\n') {
		int rows = offset / (2*MAX_COLS);
		offset = get_screen_offset(79, rows);
	} else {
		video_memory[offset] = character;
		video_memory[offset+1] = attribute_byte;
	}

	offset += 2;
	offset = handle_scrolling(offset);
	set_cursor(offset);
}

void print_at(char *message, int col, int row)  {

	if(col >= 0 && row >= 0) {
		set_cursor(get_screen_offset(col,row));
	}

	int i = 0;
	while(message[i] != 0) {
		if(col < 0 && row < 0) {
			print_char_serial(message[i]);
		}
		print_char(message[i++], col, row, WHITE_ON_BLACK);
	}
}

void printk(char *s){
	while(*s) {
		print_char_serial(*s);
		print_char(*s, -1, -1, WHITE_ON_BLACK);
		s++;
	}
}

int get_cursor() {
	int offset = 0;
	port_byte_out(REG_SCREEN_CTRL , 14);
	offset = port_byte_in(REG_SCREEN_DATA) << 8;
	port_byte_out(REG_SCREEN_CTRL , 15);
	offset |= port_byte_in(REG_SCREEN_DATA);
	return offset*2;
}

int get_screen_offset(int col, int row) {
	return ((row * MAX_COLS) + col) * 2;
}

void set_cursor(int offset) {
	offset /= 2;
	port_byte_out ( REG_SCREEN_CTRL , 14);
	port_byte_out ( REG_SCREEN_DATA , ( unsigned char )( offset >> 8));
	port_byte_out ( REG_SCREEN_CTRL , 15);
	port_byte_out ( REG_SCREEN_DATA , ( unsigned char )offset);
}

int handle_scrolling( int cursor_offset) {

	if(cursor_offset < MAX_ROWS*MAX_COLS*2) {
		return cursor_offset;
	}

	for(int i = 1; i < MAX_ROWS; i++) {
		memcpy((void *) get_screen_offset(0, i-1) + VIDEO_ADDRESS,
				(void *) get_screen_offset(0, i) + VIDEO_ADDRESS,
				MAX_COLS*2) ;
	}

	char *last_line = (char *)get_screen_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
	for(int i = 0; i < MAX_COLS*2; i++) {
		last_line[i] = 0;
	}

	cursor_offset -= 2*MAX_COLS;

	return cursor_offset;
}

void clear_screen() {

	for(int row=0; row<MAX_ROWS; row++) {
		for(int col=0; col<MAX_COLS; col++) {
			print_char(' ', col, row, WHITE_ON_BLACK);
		}
	}

	set_cursor(get_screen_offset(0,0));
}

