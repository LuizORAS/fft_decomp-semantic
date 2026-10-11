/* LIBGPU 80023b7c-80023b98. */
#include "psx/libgpu.h"

void* NextPrim(void* primitive) {
    return (void*)(getaddr(primitive) | 0x80000000);
}
