#ifndef __HEAP_H__
#define __HEAP_H__

void init_heap();
void *kmalloc(int size);
void kfreee(void *ptr);

#endif // __HEAP_H__
