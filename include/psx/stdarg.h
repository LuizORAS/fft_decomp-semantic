#ifndef PSX_STDARG_H
#define PSX_STDARG_H

/* Psy-Q GCC arguments occupy whole 32-bit int slots. */
typedef char* va_list;

#define __va_rounded_size(type) (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(ap, last)      (ap = (char*)&(last) + __va_rounded_size(last))
#define va_arg(ap, type)        (ap += __va_rounded_size(type), *(type*)(ap - __va_rounded_size(type)))
#define va_end(ap)              (ap = (char*)0)

#endif
