#include "fft/battle.h"
#include "psx/types.h"

void battle_map_stop_palette_animation(s32 clut_row, s32 clut_index, s32 stop_all) {
    s32 index;

    if (stop_all == 1) {
        index = MAP_CLUTS_PER_ROW - 1;
        do {
            g_battle_map_palette_state.clut_rows[clut_row].animations[index].active = 0;
            index--;
        } while (index >= 0);
    } else {
        g_battle_map_palette_state.clut_rows[clut_row].animations[clut_index].active = 0;
    }
}
