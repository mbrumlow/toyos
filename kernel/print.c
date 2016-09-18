
#include <kernel/kernel.h>
#include <kernel/string.h>
#include <kernel/print.h>
#include <kernel/screen.h>

#define ZEROPAD 1
#define SIGN    2

char *number(int num, char *str, int base, int width, int precision, int flags) {

	int i = 0;
	int neg = 0;
	char s[12]; // 12 should be fine for 32bits with all the extras.
	unsigned int n;

	if(base < 2 || base > 16)
		return NULL;

	if(num == 0) {
		s[i++] = '0';
	}

	if(num < 0 && base == 10 && flags & SIGN) {
		neg = 1;
		n = -num;
		width--;
	} else
		n = (unsigned int) num;

	while(n != 0) {
		unsigned int rem = n % base;
		s[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		n = n/base;
	}

	if(flags & ZEROPAD) {
		while(i < width)
			s[i++] = '0';
	}

	if(neg && flags & SIGN)
		s[i++] = '-';

	while(i > 0)
		*str++ = s[--i];

	return str;
}

static int vkprintf_atoi(const char **s) {
	int i = 0;
	while (isnumber(**s))
		i = i * 10 + *((*s)++) - '0';
	return i;
}

void vsprintf(char *buf, const char *fmt, va_list args) {

	int flags, base, len;
	int precision, width;
	unsigned long num;
	char *str;
	char *s;

	for(str = buf; *fmt; ++fmt) {

		if(*fmt != '%') {
			*str++ = *fmt;
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

		width = -1;
		if(isnumber(*fmt)) {
			width = vkprintf_atoi((const char **)&fmt);
		}

		precision = -1;
		if(*fmt == '.') {
			fmt++;
			if(isnumber(*fmt)) {
				precision = vkprintf_atoi((const char **)&fmt);
			} else if (*fmt == '*')  {
				precision = va_arg(args, int);
			}
			if(precision < 0) {
				precision = 0;
			}
		}

		base = 10;
		switch(*fmt) {

			case 's':
				s = va_arg(args, char *);
				len = strnlen(s, precision);

				while(len--) {
					*str++ = *s++;
				}

				continue;
			case 'c':
				*str++ = va_arg(args, char);
				continue;
			case '%':
				*str++ = *fmt;
				continue;
			case 'p':
				if(width == -1) {
					width = 2 * sizeof(void *) ;
					flags |= ZEROPAD;
				}
				str = number((unsigned int) va_arg(args, void *),
					     str, 16, width, precision, flags);
				continue;
			case 'x':
			case 'X':
				base = 16;
				break;
			case 'd':
			case 'i':
				flags |= SIGN;
			case 'u':
				break;
			defaut:
				continue;
		}

		if( flags & SIGN )
			num = va_arg(args, int);
		else
			num = (unsigned long) va_arg(args, unsigned int);

		str = number(num, str, base, width, precision, flags);
	}
	*str = '\0';
}

void kprintf(const char *fmt, ... ) {

	char buf[1024];

	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	printk(buf);

}

