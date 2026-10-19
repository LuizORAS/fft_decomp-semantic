#include "fft/world.h"

/* WORLD twin of battle_menu_init_primitive_colors_normal. */
void world_menu_init_primitive_colors_normal(world_menu_palette_primitives_t* menu) {
    s32 index;
    s32 palette_index;
    LINE_F2* line;

    line = menu->lines;
    for (index = 0; index < 12; index++) {
        palette_index = g_world_menu_numeric_display_palette_indices[index];
        line->r0 = g_world_menu_palette_colors.colors[palette_index].red << 3;
        line->g0 = g_world_menu_palette_colors.colors[palette_index].green << 3;
        line->b0 = g_world_menu_palette_colors.colors[palette_index].blue << 3;
        line++;
    }
    menu->sprite.clut = 0x7c3c;
}
