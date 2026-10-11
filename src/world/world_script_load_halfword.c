#include "fft/world.h"

s16 world_script_load_halfword(const void* source) {
    const u8* bytes = source;

    return (bytes[0] | (bytes[1] << 8));
}
