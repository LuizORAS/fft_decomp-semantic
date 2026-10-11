#include "fft/world.h"
#include "psx/types.h"

/* Store a 16-bit value at an unaligned script address, low byte first. */
void world_script_store_halfword(u8* destination, s16 value) {
    destination[0] = value;
    destination[1] = value >> 8;
}
