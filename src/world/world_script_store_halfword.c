#include "psx/types.h"

void world_script_store_halfword(u8* destination, s16 value) {
    destination[0] = value;
    destination[1] = value >> 8;
}
