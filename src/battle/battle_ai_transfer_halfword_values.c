#include "fft/battle_ai.h"
#include "psx/types.h"

/* Copy byte_count / 2 16-bit values; an odd last byte is left alone. */
void battle_ai_transfer_halfword_values(void* destination, void* source, s32 byte_count) {
    u16* dst = destination;
    u16* src = source;
    s32 count = byte_count / 2;
    s32 i;
    for (i = 0; i < count; i++) {
        *dst++ = *src++;
    }
}
