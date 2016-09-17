#ifndef __KERNEL_H__
#define __KERNEL_H__

typedef char *va_list;

#define NULL ((char *)0)
#define _bnd(X, bnd)            (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)           (*(T *)(((ap) += (_bnd (T, 3))) - (_bnd (T,3))))
#define va_end(ap)              (ap = (va_list) NULL)
#define va_start(ap, A)         (void) ((ap) = (((char *) &(A)) + (_bnd (A,3))))

#endif // __KERNEL_H__
