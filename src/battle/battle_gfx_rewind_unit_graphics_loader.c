#include "fft/battle.h"
#include "psx/types.h"

/* Rewinds the unit graphics loader (g_battle_gfx_unit_graphics_load_state) to its
 * first file kind, the start-read phase, index 0 and no buffer. The argument is
 * not read. */
void battle_gfx_rewind_unit_graphics_loader(s32 unused_map_id) {
    g_battle_gfx_unit_graphics_load_state[0] = BATTLE_GFX_UNIT_LOAD_KIND_FIRST;
    g_battle_gfx_unit_graphics_load_state[1] = BATTLE_GFX_UNIT_LOAD_PHASE_START_READ;
    g_battle_gfx_unit_graphics_load_state[2] = 0;
    g_battle_gfx_unit_graphics_load_state[3] = 0;
}
