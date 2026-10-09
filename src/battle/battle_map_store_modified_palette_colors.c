#include "fft/battle.h"
#include "psx/types.h"

void battle_map_store_modified_palette_colors(s32 bank, s32 row, s32 store_all) {
    if (store_all == 1) {
        u16* destination;
        u16* source;

        store_all = 0;
        destination = g_battle_map_palette_state.banks[bank].packed_colors;
        source = g_battle_map_palette_modified_colors[0];
        source += bank * 0x100;
        while (store_all < 256) {
            *destination++ = *source++;
            store_all++;
        }
    } else {
        u16* stored_palette_group;

        store_all = 0;
        row *= 16;
        stored_palette_group = g_battle_map_palette_state.banks[bank].packed_colors;
        while (store_all < 16) {
            stored_palette_group[row + store_all] = g_battle_map_palette_modified_colors[bank][row + store_all];
            store_all++;
        }
    }
}
