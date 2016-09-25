#ifndef __PAGE_H__
#define __PAGE_H__

#define PAGE_SIZE 4096

void init_paging(void *top);
int mapped(void *virtaddr);
void kmap(void *physaddr, void *virtaddr);
void kunmap(void *virtaddr);

#endif //__PAGE_H__
