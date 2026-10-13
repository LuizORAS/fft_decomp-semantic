#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_alloc_buffer. */
void* world_menu_alloc_buffer(s32 size) {
    return world_menu_alloc_memory(size);
}
