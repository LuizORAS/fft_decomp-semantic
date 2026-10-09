#include "fft/battle.h"
#include "psx/types.h"

void battle_gfx_update_unit_palettes(void) {
    s32 palette_index;

    for (palette_index = 0; palette_index < 0x10; palette_index++) {
        if (g_battle_map_palette_state.banks[3].animations[palette_index].active != 0) {
            battle_map_modify_palette(10, 4, 3, palette_index, 0, 0, 0, 0);
            battle_map_modify_palette(8, 4, 3, palette_index, 0, 0, 0, 0);
        }
    }
}
