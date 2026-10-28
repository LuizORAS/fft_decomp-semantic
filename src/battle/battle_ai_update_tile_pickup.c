#include "fft/battle_ai.h"
#include "fft/battle_unit.h"
#include "psx/types.h"

/* Note what the simulation picks up at the current action's destination
 * (battle_ai_simulate_movement_and_pickup): tile_pickup_kind 1 for a Crystal, 2 for a Treasure
 * (it wins), 0 for neither or no unit. */
void battle_ai_update_tile_pickup(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 unit_id;
    battle_stats_t* unit;

    g_battle_ai_data_base.tile_pickup_kind = BATTLE_AI_TILE_PICKUP_NONE;
    unit_id = battle_ai_find_unit_at_coordinates(&ai->current_action.coords);
    if (unit_id != BATTLE_UNIT_ID_NONE) {
        unit = &g_battle_unit_stats[unit_id];
        if (unit->entd_slot != BATTLE_ENTD_SLOT_NONE) {
            if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRYSTAL)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRYSTAL)) {
                g_battle_ai_data_base.tile_pickup_kind = BATTLE_AI_TILE_PICKUP_CRYSTAL;
            }
            if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TREASURE)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TREASURE)) {
                g_battle_ai_data_base.tile_pickup_kind = BATTLE_AI_TILE_PICKUP_TREASURE;
            }
        }
    }
}
