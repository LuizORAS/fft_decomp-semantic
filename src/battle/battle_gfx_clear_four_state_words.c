#include "fft/battle.h"
#include "psx/types.h"

void battle_gfx_clear_four_state_words(s32 unused_map_id) {
    g_battle_gfx_state_words[0] = 0;
    g_battle_gfx_state_words[1] = 0;
    g_battle_gfx_state_words[2] = 0;
    g_battle_gfx_state_words[3] = 0;
}
