#ifndef __PAGE_H__
#define __PAGE_H__

void init_paging(void *pd);
void *get_physaddr(void *virtaddr);



#endif //__PAGE_H__
