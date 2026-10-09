#include "fft/battle.h"
#include "psx/types.h"

u8* battle_gfx_get_effect_seq_data(u32 unused_spritesheet_id) {
    return g_battle_gfx_effect_seq_data;
}
