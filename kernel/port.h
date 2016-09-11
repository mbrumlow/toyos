#ifndef __PORT_H__
#define __PORT_H__

unsigned char port_byte_in(unsigned short port);
unsigned char port_byte_out(unsigned short port, unsigned char data);
unsigned char port_word_in(unsigned short port);
unsigned char port_word_out(unsigned short port, unsigned short data);

#endif // __PORT_H__
