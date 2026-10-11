/* LIBGPU 80026f58-80026f84. */
#include "psx/libgpu.h"

void memset2(void* destination, int value, int size) {
    u8* p = destination;
    while (size--) {
        *p++ = value;
    }
}
