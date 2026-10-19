#include "fft/memory.h"
#include "fft/world.h"

/* WORLD twin of battle_menu_copy_palette_colors. */
void world_menu_copy_palette_colors(const menu_palette_pair_t* source) {
    mem_copy_bytes_nonempty(g_world_menu_palette_colors.colors, source->colors, sizeof(palette_t));
    mem_copy_bytes_nonempty(g_world_menu_palette_colors.colors + PALETTE_COLOR_COUNT,
        source->colors + PALETTE_COLOR_COUNT, sizeof(palette_t));
}
