#include "fft/battle.h"

void battle_gfx_set_evtchr_palette_by_misc_id(s32 palette_row_index, s32 slot_index, s32 clut_index) {
    battle_map_load_palette_data(
        g_battle_gfx_vram_slots[slot_index].cluts[clut_index], MAP_PALETTE_BANK_UNITS, palette_row_index, 0);
    battle_map_load_palette_data(
        g_battle_gfx_vram_slots[slot_index].cluts[clut_index + 8], MAP_PALETTE_BANK_UNITS_SECOND, palette_row_index, 0);
}
