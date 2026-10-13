#include "fft/script.h"
#include "psx/types.h"

/* Store a 16-bit value at an unaligned script address, low byte first. */
void script_write_s16(u8* destination, s16 value) {
    destination[0] = value;
    destination[1] = value >> 8;
}
