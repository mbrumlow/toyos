
#include <kernel/kernel.h>
#include <kernel/string.h>
#include <kernel/print.h>
#include <kernel/screen.h>



char *itoa(int num, char *str, int base) {

	int neg = 0;
	char *s = str;


	if(num == 0) {
		*(s++) = '0';
		*(s++) = '\0';
		return str;
	}

	if(num < 0 && base == 10) {
		neg = 1;
		num = -num;
	}

	while(num != 0) {
		int rem = num % base;
		*(s++) = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num/base;
	}

	*(s++) = '\0';
	reverse(str);

	return str;
}

void vkprintf(char *fmt, va_list args) {

	int base, len;
	unsigned long num;
	char *s;
	char str[100];

	for(; *fmt; ++fmt) {

		if(*fmt != '%') {
			printk(*fmt);
			continue;
		}

		++fmt;

		base = 10;

		char *s = str;
		switch(*fmt) {

			case 's':
				s = va_arg(args, char *);
				while(*s)
					printk(*(s++));
				continue;

			case 'c':
				printk(va_arg(args, char));
				continue;
			case '%':
				printk(*fmt);
				continue;
			case 'o':
				base = 8;
				break;
			case 'x':
			case 'X':
				base = 16;
				break;
			case 'd':
			case 'i':
				break;
			defaut:
				continue;
		}

		num = va_arg(args, int);
		itoa(num, s, base);

		s = str;
		while(*s)
			printk(*(s++));
	}
}

void kprintf(char *fmt, ... ) {
	va_list args;
	va_start(args, fmt);
	vkprintf(fmt, args);
	va_end(args);
}

