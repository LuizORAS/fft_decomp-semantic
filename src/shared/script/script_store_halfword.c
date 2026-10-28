#include "fft/script.h"
#include "psx/types.h"

/* Write a 16-bit value little-endian into a script operand, which may be unaligned. BATTLE and WORLD
 * compile this text. */
void script_store_halfword(u8* destination, s16 value) {
    destination[0] = value;
    destination[1] = value >> 8;
}
