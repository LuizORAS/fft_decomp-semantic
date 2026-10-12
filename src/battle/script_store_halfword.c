#include "fft/battle_script.h"
#include "psx/types.h"

/* Write a 16-bit value little-endian into a script operand, which may be unaligned. */
void script_store_halfword(u8* dst, s16 value) {
    dst[0] = value & 0xff;
    dst[1] = value >> 8;
}
