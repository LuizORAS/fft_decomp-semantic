#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_map_update_palette_colors for one CLUT of the unit row. A clut_index of 16 or more
 * applies the same sixteen colours to every unit CLUT. */
void battle_map_update_palette_colors_by_index(s32 mode, s32 clut_index, const void* color_data) {
    s32 index;

    if (clut_index >= 16) {
        index = 0;
        do {
            battle_map_update_palette_colors(mode, MAP_CLUT_ROW_UNITS, index, 0, color_data);
            index++;
        } while (index < 16);
    } else {
        battle_map_update_palette_colors(mode, MAP_CLUT_ROW_UNITS, clut_index, 0, color_data);
    }
}
