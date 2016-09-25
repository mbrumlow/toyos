#ifndef __HEAP_H__
#define __HEAP_H__

void init_heap();
void *kmalloc(int size);
void kfree(void *ptr);

#endif // __HEAP_H__
