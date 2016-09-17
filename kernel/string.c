
int strlen(char *s) {
	int i = 0;
	while(*(s++))
		i++;
	return i;
}

void reverse(char *s) {
	char t;
	char *e = s + strlen(s) - 1;
	while( s < e ) {
		t = *s;
		*(s++) = *e;
		*(e--) = t;
	}
}

void *memcpy(void *dest, void *src, int n) {
	unsigned char *d = (unsigned char *) dest;
	unsigned char *s = (unsigned char *) src;
	for( int i = 0; i < n; i++)
		*(d++) = *(s++);
	return dest;
}


