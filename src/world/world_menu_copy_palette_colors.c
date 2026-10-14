#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_copy_palette_colors. */
void world_menu_copy_palette_colors(const u8* source) {
    world_util_copy_bytes(g_world_menu_palette_colors, source, 0x20);
    world_util_copy_bytes(g_world_menu_palette_colors + 0x20, source + 0x20, 0x20);
}
