#include "fft/battle.h"
#include "psx/types.h"

void battle_map_store_modified_palette_colors(s32 clut_row, s32 clut_index, s32 store_all) {
    if (store_all == 1) {
        u16* destination;
        u16* source;

        store_all = 0;
        destination = g_battle_map_palette_state.clut_rows[clut_row].packed_colors;
        source = g_battle_map_palette_modified_colors[0];
        source += clut_row * 0x100;
        while (store_all < 256) {
            *destination++ = *source++;
            store_all++;
        }
    } else {
        u16* packed_row;

        store_all = 0;
        clut_index *= 16;
        packed_row = g_battle_map_palette_state.clut_rows[clut_row].packed_colors;
        while (store_all < 16) {
            packed_row[clut_index + store_all] = g_battle_map_palette_modified_colors[clut_row][clut_index + store_all];
            store_all++;
        }
    }
}
