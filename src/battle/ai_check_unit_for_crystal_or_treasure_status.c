#include "fft/battle_ai.h"
#include "fft/battle_unit.h"
#include "psx/types.h"

/* Classify the crystal or treasure occupant of the current action tile. */
void ai_check_unit_for_crystal_or_treasure_status(void) {
    ai_data_t* ai = &g_ai_data_base;
    s32 unit_id;
    battle_stats_t* unit;

    g_ai_data_base.tile_pickup_kind = AI_TILE_PICKUP_NONE;
    unit_id = ai_find_unit_at_coordinates(&ai->current_action.coords);
    if (unit_id != BATTLE_UNIT_ID_NONE) {
        unit = &g_battle_unit_stats[unit_id];
        if (unit->entd_slot != BATTLE_ENTD_SLOT_NONE) {
            if (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CRYSTAL)] & STATUS_BYTE_MASK(STATUS_ID_CRYSTAL)) {
                g_ai_data_base.tile_pickup_kind = AI_TILE_PICKUP_CRYSTAL;
            }
            if (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_TREASURE)]
                & STATUS_BYTE_MASK(STATUS_ID_TREASURE)) {
                g_ai_data_base.tile_pickup_kind = AI_TILE_PICKUP_TREASURE;
            }
        }
    }
}
