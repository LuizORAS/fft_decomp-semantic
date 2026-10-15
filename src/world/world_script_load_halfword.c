#include "fft/world.h"

/* Read a little-endian halfword from a script operand, which may be unaligned. */
s16 world_script_load_halfword(const u8* source) {
    return (source[0] | (source[1] << 8));
}
