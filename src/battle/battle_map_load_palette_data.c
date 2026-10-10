#include "fft/battle.h"
#include "psx/types.h"

/* Load colours into a map CLUT row and mark the rows for upload: the row's
 * sixteen CLUTs (256 colours) when load_all is 1, otherwise the sixteen colours
 * of CLUT clut_index. The blends of the loaded CLUTs stop. Nothing happens while
 * the row's updates are disabled. */
void battle_map_load_palette_data(const u16* palette, s32 clut_row, s32 clut_index, s32 load_all) {
    s32 clear_index;
    s32 i;
    const u16* clut;

    if (g_battle_map_palette_state.clut_rows[clut_row].updates_disabled != 0) {
        return;
    }

    if (load_all == 1) {
        clear_index = MAP_CLUTS_PER_ROW - 1;
        do {
            g_battle_map_palette_state.clut_rows[clut_row].animations[clear_index].active = 0;
            clear_index--;
        } while (clear_index >= 0);

        i = 0;
        clut = palette;
        do {
            battle_map_extract_palette_colors(clut, clut_row, i);
            i++;
            clut += MAP_CLUT_COLOR_COUNT;
        } while (i < MAP_CLUTS_PER_ROW);
    } else {
        g_battle_map_palette_state.clut_rows[clut_row].animations[clut_index].active = 0;
        battle_map_extract_palette_colors(palette, clut_row, clut_index);
    }

    g_battle_map_palette_state.upload_pending = 1;
}
