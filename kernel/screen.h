#ifndef __SCREEN_H__
#define __SCREEN_H__

void clear_screen();
void printk(char *message);
void print_char(char character, int col, int row, char attribute_byte);

#endif // __SCREEN_H__
