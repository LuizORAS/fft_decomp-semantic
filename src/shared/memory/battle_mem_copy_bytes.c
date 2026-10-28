#include "fft/memory.h"

/* Copy len bytes from src to dst (nothing when len is 0 or less). */
void battle_mem_copy_bytes(void* destination, const void* source, s32 len) {
    u8* dst = destination;
    const u8* src = source;
    s32 end;
    if (len > 0) {
        end = len + (s32)dst;
        do {
            *dst = *src;
            dst = dst + 1;
            src = src + 1;
        } while ((s32)dst < end);
    }
}
