
int strlen(char *s) {
	int i = 0;
	while(*(s++))
		i++;
	return i;
}

int strnlen(char *s, int n) {
	int i = 0;
	while(*(s++) && n--)
		i++;
	return i;
}

int isnumber(const char c) {
	if( c >= '0' && c <= '9' )
		return 1;
	return 0;
}


void *memcpy(void *dest, void *src, int n) {
	unsigned char *d = (unsigned char *) dest;
	unsigned char *s = (unsigned char *) src;
	for( int i = 0; i < n; i++)
		*(d++) = *(s++);
	return dest;
}


