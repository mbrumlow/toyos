
#include <kernel/kernel.h>

#define LAST_ENTRY(n) (n && !(((unsigned int)(n + sizeof(void *))) & 0x00000FFF))
#define FIRST_ENTRY(n) (n && ((unsigned int)n & 0x00000FFF) == 0 )

static void *top = 0;
static unsigned int *next = 0;

void init_memory(void *end) {
	top = end;
}

void *new_frame()  {
	void *ptr = top;
	top += 4096;
	return ptr;
}

void *alloc_frame() {

	void *ptr;

	if(next) {

		if (FIRST_ENTRY(next)) {

			if(*next) {
				ptr = next;
				next = (unsigned int *) *next;
				return ptr;
			}

			ptr = (void *) next;
			next = 0;

			return ptr;
		} else {
			ptr = (void *) *next;
			next--;
			return ptr;
		}
	}

	return new_frame();
}


void put_frame(void *addr)  {

	if(!next) {
		next = (unsigned int *) addr;
		*next = 0;
		return;
	} else if(LAST_ENTRY(next)) {
		unsigned int tmp = (unsigned int) next;
		next = (unsigned int *) addr;
		*next = tmp;
		return;
	}

	next++;
	*next = (unsigned int) addr;
}


