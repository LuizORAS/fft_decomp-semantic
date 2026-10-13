#include "fft/memory.h"

void mem_copy_byte_data_source_first(const void* source, void* destination, s32 count) {
    const u8* src = source;
    u8* dst = destination;
    s32 index = 0;

    while (index < count) {
        *dst++ = *src++;
        index++;
    }
}
