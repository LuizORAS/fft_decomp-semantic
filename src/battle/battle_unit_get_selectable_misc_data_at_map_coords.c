#include "fft/battle_action.h"
#include "fft/battle_unit.h"

/* Returns the unit the cursor selects on a tile: one with battle stats in an
 * occupied slot that is not a crystal, a treasure or in the air from Jump.
 *
 * Among several, the source unit (g_battle_unit_source_misc_id) is preferred,
 * then a rider over its mount, then the first in list order. Returns 0 for none.
 * The candidate array holds 16 entries and is filled without a bound check. */
battle_unit_misc_data_t* battle_unit_get_selectable_misc_data_at_map_coords(s32 map_x, s32 map_y, s32 map_z) {
    battle_unit_misc_data_t* matches[BATTLE_UNIT_MISC_SLOT_COUNT];
    battle_unit_misc_data_t* unit;
    s32 count;
    s32 index;

    count = 0;
    unit = g_battle_unit_misc_list_head;
    while (unit != 0) {
        if ((unit->map_x == map_x) && (unit->map_y == map_y) && (unit->map_z == map_z) && (unit->battle_data != 0)
            && (unit->battle_data->entd_slot != BATTLE_ENTD_SLOT_NONE)
            && ((unit->status_flags_5_6
                    & (BATTLE_MISC_STATUS_CRYSTAL | BATTLE_MISC_STATUS_TREASURE | BATTLE_MISC_STATUS_JUMP))
                == 0)) {
            matches[count++] = unit;
        }
        unit = unit->previous;
    }
    if (count != 0) {
        if (count != 1) {
            for (index = 0; index < count; index++) {
                if (matches[index]->unit_id == g_battle_unit_source_misc_id) {
                    return matches[index];
                }
            }
            for (index = 0; index < count; index++) {
                if (matches[index]->mount_state == BATTLE_MISC_MOUNT_STATE_RIDER) {
                    return matches[index];
                }
            }
        }
        return matches[0];
    }
    return 0;
}
