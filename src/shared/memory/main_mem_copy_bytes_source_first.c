#include "fft/memory.h"

/* Copy `count` bytes from `source` to `destination`, one at a time (the source comes first, unlike
 * memcpy). MAIN compiles this text twice: main_mem_copy_bytes_source_first (0x8005cc64), which the unit
 * generation and update routines (main_unit_*) call, and main_mem_copy_byte_data_source_first, with
 * which BATTLE's action, AI, formula, movement and status code copies its records. */
void main_mem_copy_bytes_source_first(const void* source, void* destination, int count) {
    const u8* src = source;
    u8* dst = destination;
    int i;

    for (i = 0; i < count; i++) {
        *dst++ = *src++;
    }
}
