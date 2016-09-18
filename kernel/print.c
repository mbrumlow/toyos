
#include <kernel/kernel.h>
#include <kernel/string.h>
#include <kernel/print.h>
#include <kernel/screen.h>


#define ZEROPAD 1
#define SIGN    2
#define PLUS    4
#define SPACE   8

char *number(int num, char *str, int base, int width, int precision, int flags) {

	int len = 0;
	int neg = 0;
	char *s = str;

	if(base < 2 || base > 16)
		return NULL;

	if(num == 0) {
		*(s++) = '0';
		len++;
	}

	if(num < 0 && base == 10) {
		neg = 1;
		num = -num;
	}

	while(num != 0) {
		int rem = num % base;
		*(s++) = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num/base;
		len++;
	}

	if(flags & ZEROPAD) {
		while(len++ < width)
			*(s++) = '0';
	}

	if(neg)
		*(s++) = '-';

	*(s++) = '\0';
	reverse(str);

	return str;
}

static int vkprintf_atoi(const char **s) {
	int i = 0;
	while (**s >= '0' && **s <= '9')
		i = i * 10 + *((*s)++) - '0';
	return i;
}

void vkprintf(char *fmt, va_list args) {

	int base, len, flags;
	int precision, width;
	unsigned long num;
	char *s;
	char str[100];

	for(; *fmt; ++fmt) {

		if(*fmt != '%') {
			printk(*fmt);
			continue;
		}

		flags = 0;
		loop:
		++fmt;

		switch(*fmt)  {
			case '0':
				flags |= ZEROPAD;
				goto loop;
		}

		base = 10;
		width = -1;
		precision = -1;

		if( *fmt >= '0' && *fmt <= '9' )  {
			width = vkprintf_atoi((const char **)&fmt);
		}

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
				num = va_arg(args, int);
				break;
			case 'p':
				base = 16;
				width = 2 * sizeof(void *) ;
				flags |= ZEROPAD;
				num = (unsigned int) va_arg(args, void *);
				break;
			case 'X':
				// TODO implement topper.
			case 'x':
				base = 16;
				num = va_arg(args, int);
				break;
			case 'd':
			case 'i':
				num = va_arg(args, int);
				break;
			defaut:
				continue;
		}

		number(num, s, base, width, precision, flags);

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

