#include "fft/battle.h"
#include "psx/types.h"

void battle_map_update_palette_colors_by_index(s32 mode, s32 row, const void* color_data) {
    s32 index;

    if (row >= 16) {
        index = 0;
        do {
            battle_map_update_palette_colors(mode, MAP_PALETTE_BANK_UNITS, index, 0, color_data);
            index++;
        } while (index < 16);
    } else {
        battle_map_update_palette_colors(mode, MAP_PALETTE_BANK_UNITS, row, 0, color_data);
    }
}
