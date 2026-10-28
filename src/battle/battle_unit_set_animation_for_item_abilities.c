#include "fft/battle_gfx.h"
#include "fft/battle_unit.h"
#include "psx/types.h"

/* Show the used item on the unit (battle_gfx_store_item_display_data) and start its use animation:
 * 0x39 on itself or on a target one tile away in a straight line, otherwise the throw (0x4c), which
 * clears item_display_active. */
void battle_unit_set_animation_for_item_abilities(battle_unit_t* unit, battle_unit_t* target) {
    s32 dx;
    s32 dy;

    battle_gfx_store_item_display_data(unit, unit->used_item_or_weapon_id);
    if (target != 0) {
        dx = target->map_x - unit->map_x;
        dy = target->map_y - unit->map_y;
    } else {
        dx = 0;
        dy = 0;
    }
    if (dx < 0) {
        dx = -dx;
    }
    if (dy < 0) {
        dy = -dy;
    }
    if ((dx <= 0 && dy <= 0) || (dx < 2 && dy == 0) || (dx == 0 && dy < 2)) {
        battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_HOLD_ITEM, unit->facing, unit);
    } else {
        unit->item_display_active = 0;
        battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_THROW_ITEM, unit->facing, unit);
    }
}
