/* LIBGPU 80023b7c-80023b98. */
#include "psx/libgpu.h"

/* Return the primitive `primitive` links to, as a cached (KSEG0) address. No code refers to it. */
void* NextPrim(void* primitive) {
    return (void*)(((P_TAG*)primitive)->addr | 0x80000000);
}
