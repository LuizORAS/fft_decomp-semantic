#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_gfx_get_unit_spritesheet_height for the unit with this misc id. */
s32 battle_gfx_get_unit_spritesheet_height_by_misc_id(u32 misc_id) {
    return battle_gfx_get_unit_spritesheet_height(battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff));
}
