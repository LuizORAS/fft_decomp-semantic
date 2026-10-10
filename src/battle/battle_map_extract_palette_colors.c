#include "fft/battle.h"
#include "psx/types.h"

enum {
    MAP_CLUT_ROW_UPLOAD_BYTES = 0x200,
};

/* Load the sixteen colours of one CLUT into a map CLUT row: the upload staging
 * (g_battle_map_palette_modified_colors), the row's packed mirror, and its
 * decoded 5-bit components and alpha bit. */
void battle_map_extract_palette_colors(const u16* palette, s32 clut_row, s32 clut_index) {
    s32 color_index;
    s32 selected_clut;
    s32 i;
    const u16* palette_ptr;
    u16* upload_palette_base;
    u16* upload_row;
    u16* packed_row;
    map_palette_color_components_t* component_row;
    u16 color;
    map_palette_color_components_t* components;

    selected_clut = clut_index;
    i = 0;
    component_row = g_battle_map_palette_state.clut_rows[clut_row].components;
    upload_palette_base = g_battle_map_palette_modified_colors[0];
    upload_row = upload_palette_base + clut_row * (MAP_CLUT_ROW_UPLOAD_BYTES / sizeof(u16));
    packed_row = g_battle_map_palette_state.clut_rows[clut_row].packed_colors;
    palette_ptr = palette;
    do {
        /* A CLUT entry is ABBBBBGGGGGRRRRR. */
        color = *palette_ptr++;
        color_index = selected_clut << 4;
        color_index += i;
        i++;
        upload_row[color_index] = color;
        components = &component_row[color_index];
        packed_row[color_index] = color;
        components->red_5bit = color & 0x1f;
        components->green_5bit = (color & 0x3e0) >> 5;
        components->blue_5bit = (color & 0x7c00) >> 10;
        components->alpha_bit = color >> 15;
    } while (i < MAP_CLUT_COLOR_COUNT);
}
