#include "fft/battle.h"
#include "psx/types.h"

void battle_gfx_clear_four_state_words(s32 unused_map_id) {
    g_battle_gfx_unit_graphics_load_state[0] = 0;
    g_battle_gfx_unit_graphics_load_state[1] = 0;
    g_battle_gfx_unit_graphics_load_state[2] = 0;
    g_battle_gfx_unit_graphics_load_state[3] = 0;
}
