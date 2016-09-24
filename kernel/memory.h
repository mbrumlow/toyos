#ifndef __MEMORY_H__
#define __MEMORY_H__

void init_memory(void *_top);
void *alloc_frame();
void put_frame(void *addr);

#endif //__MEMORY_H__

