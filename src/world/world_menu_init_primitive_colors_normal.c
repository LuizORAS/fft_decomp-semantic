#include "fft/world.h"

/* Give the twelve frame lines the colours of the normal menu palette (the first
 * sixteen of the menu palette colours) and the sprite the normal menu CLUT, GetClut(0x3c0, 0x1f0). */
void world_menu_init_primitive_colors_normal(world_menu_palette_primitives_t* menu) {
    s32 index;
    s32 palette_offset;
    LINE_F2* line;

    index = 0;
    line = menu->lines;
    do {
        palette_offset = g_world_numeric_display_palette_indices[index] * 2;
        line->r0 = (*(g_world_menu_palette_colors + palette_offset) & 0x1f) << 3;
        line->g0 = (*(u16*)(g_world_menu_palette_colors + palette_offset) >> 2) & 0xf8;
        index++;
        line->b0 = (*(u16*)(g_world_menu_palette_colors + palette_offset) >> 7) & 0xf8;
        line++;
    } while (index < 12);
    menu->sprite.clut = 0x7c3c;
}
