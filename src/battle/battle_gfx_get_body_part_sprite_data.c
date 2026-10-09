#include "fft/battle.h"
#include "psx/types.h"

u8* battle_gfx_get_body_part_sprite_data(u32 unused_spritesheet_id) {
    return g_battle_gfx_body_part_sprite_data;
}
