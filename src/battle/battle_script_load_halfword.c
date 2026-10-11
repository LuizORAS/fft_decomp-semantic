#include "fft/battle.h"

/* Read a little-endian halfword from a script operand, which may be unaligned. */
s16 battle_script_load_halfword(const u8* data) {
    return (data[0] | (data[1] << 8));
}
