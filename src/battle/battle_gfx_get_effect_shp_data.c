#include "fft/battle.h"
#include "psx/types.h"

/* Return the effect SHP of a sheet: every sheet shares the one file, EFF1. */
u8* battle_gfx_get_effect_shp_data(u32 unused_spritesheet_id) {
    return g_battle_gfx_effect_shp_data;
}
