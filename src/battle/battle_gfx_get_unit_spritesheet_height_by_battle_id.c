#include "fft/battle_gfx.h"
#include "psx/types.h"

struct battle_unit;

/* Same as battle_gfx_get_unit_spritesheet_height for the unit in battle slot `battle_id`. */
s32 battle_gfx_get_unit_spritesheet_height_by_battle_id(u32 battle_id) {
    return battle_gfx_get_unit_spritesheet_height(battle_unit_get_by_battle_id(battle_id & 0xffff));
}
