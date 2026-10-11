#include "fft/world.h"

s16 world_script_load_halfword(const u8* source) {
    return (source[0] | (source[1] << 8));
}
