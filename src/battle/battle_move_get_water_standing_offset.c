#include "fft/battle.h"

/* Return how far above a water tile's floor the unit stands, in half-heights: the whole depth
 * (on the surface) with Float or ON_WATER movement, one level less with IN_WATER movement, else
 * 0. */
s32 battle_move_get_water_standing_offset(battle_unit_misc_data_t* unit, map_tile_t* tile) {
    s32 movement_flags;

    if (unit->battle_data != 0) {
        movement_flags = battle_move_get_effective_flags(unit->battle_data);
    } else {
        movement_flags = 0;
    }
    if (tile->depth_half_height.value & MAP_TILE_DEPTH_MASK) {
        if ((movement_flags & BATTLE_EFFECTIVE_MOVEMENT_FLOAT)
            || (movement_flags & BATTLE_EFFECTIVE_MOVEMENT_ON_WATER)) {
            return (tile->depth_half_height.value >> MAP_TILE_DEPTH_SHIFT) * 2;
        }
        if (movement_flags & BATTLE_EFFECTIVE_MOVEMENT_IN_WATER) {
            return ((tile->depth_half_height.value >> MAP_TILE_DEPTH_SHIFT) - 1) * 2;
        }
    }
    return 0;
}
