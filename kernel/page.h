#ifndef __PAGE_H__
#define __PAGE_H__

#define PAGE_SIZE 4096

void init_paging(void *start, void *end);

//void *get_physaddr(void *virtaddr);

#endif //__PAGE_H__
