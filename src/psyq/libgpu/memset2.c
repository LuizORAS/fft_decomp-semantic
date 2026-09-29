/* LIBGPU 80026f58-80026f84. */
#include "psx/libgpu.h"

void memset2(void* destination, int value, int size) {
    int unused[2]; /* The original leaf reserves eight stack bytes. */
    u8* p = destination;
    int remaining = size - 1;
    if (size) {
        do {
            *p++ = value;
        } while (remaining-- != 0);
    }
}
