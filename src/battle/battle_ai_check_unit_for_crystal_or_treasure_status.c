#include "fft/battle.h"
#include "psx/types.h"

/* Set crystal_treasure_status for the unit on the current action's tile: 0
 * none, 1 crystal, 2 treasure. */
void battle_ai_check_unit_for_crystal_or_treasure_status(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 unit_id;
    battle_stats_t* unit;

    g_battle_ai_data_base.crystal_treasure_status = 0;
    unit_id = battle_ai_find_unit_at_coordinates(&ai->current_action.coords);
    if (unit_id != BATTLE_UNIT_ID_NONE) {
        unit = &g_battle_unit_stats[unit_id];
        if (unit->entd_slot != BATTLE_ENTD_SLOT_NONE) {
            if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRYSTAL)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRYSTAL)) {
                g_battle_ai_data_base.crystal_treasure_status = 1;
            }
            if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TREASURE)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TREASURE)) {
                g_battle_ai_data_base.crystal_treasure_status = 2;
            }
        }
    }
}
