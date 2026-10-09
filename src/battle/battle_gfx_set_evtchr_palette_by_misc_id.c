#include "fft/battle.h"

void battle_gfx_set_evtchr_palette_by_misc_id(s32 misc_id, s32 slot_index, s32 clut_index) {
    battle_map_load_palette_data(g_battle_gfx_vram_slots[slot_index].cluts[clut_index], MAP_CLUT_ROW_UNITS, misc_id, 0);
    battle_map_load_palette_data(
        g_battle_gfx_vram_slots[slot_index].cluts[clut_index + 8], MAP_CLUT_ROW_UNITS_SECOND, misc_id, 0);
}
