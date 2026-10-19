#include "fft/battle_gfx.h"
#include "psx/types.h"

/* Return the effect SEQ table of a sheet: every sheet shares the one table, EFF1. */
u8** battle_gfx_get_effect_seq_data(u32 unused_spritesheet_id) {
    return g_battle_gfx_effect_shp_data[0].seq;
}
