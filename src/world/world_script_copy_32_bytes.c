#include "fft/world.h"
#include "psx/types.h"

/* WORLD's copy of battle_script_copy_32_bytes: copy one 32-byte matrix (world_copy_bytes). */
void world_script_copy_32_bytes(void* destination, const void* source) {
    world_copy_bytes(destination, source, 32);
}
