#include "fft/script.h"

/* Read a little-endian 16-bit value from a script operand, which may be unaligned. */
s16 script_load_halfword(const void* source) {
    const u8* bytes = source;

    return (bytes[0] | (bytes[1] << 8));
}
