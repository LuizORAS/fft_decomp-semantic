#include "fft/battle.h"
#include "psx/types.h"

/* Why unit cannot act (battle_unit_action_block_e): DISABLED under Don't Act or as a mount carrying
 * its rider; SUBMERGED at water depth 2 or more, unless it has Float, Chicken or Frog, a water
 * movement ability (Walk on Water, Move in Water, Float) or rides a mount; otherwise NONE. The menus
 * grey the action out with it; evasion, Weapon Guard, Abandon and reactions need NONE. */
s32 battle_unit_get_action_block(battle_stats_t* unit) {
    s32 depth;

    if ((unit->status_sets.current[4] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DONT_ACT)) != 0) {
        return BATTLE_UNIT_ACTION_BLOCK_DISABLED;
    }
    if ((unit->mount_info & BATTLE_MOUNT_INFO_FLAG_MOUNT) != 0) {
        return BATTLE_UNIT_ACTION_BLOCK_DISABLED;
    }
    depth = g_battle_map_tile_data[battle_map_calculate_location(unit)].depth_half_height >> MAP_TILE_DEPTH_SHIFT;
    if (depth < 2) {
        return BATTLE_UNIT_ACTION_BLOCK_NONE;
    }
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FLOAT)]
            & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FLOAT) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHICKEN)
                | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG)))
        != 0) {
        return BATTLE_UNIT_ACTION_BLOCK_NONE;
    }
    if ((unit->movement_abilities[2]
            & (BATTLE_MOVEMENT_SET_3_WALK_ON_WATER | BATTLE_MOVEMENT_SET_3_MOVE_IN_WATER | BATTLE_MOVEMENT_SET_3_FLOAT))
        != 0) {
        return BATTLE_UNIT_ACTION_BLOCK_NONE;
    }
    /* BATTLE_UNIT_ACTION_BLOCK_SUBMERGED, or NONE for a rider. */
    return ((unit->mount_info & BATTLE_MOUNT_INFO_FLAG_RIDER) == 0) << 1;
}
