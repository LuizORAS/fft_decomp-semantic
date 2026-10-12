#include "fft/battle_ai.h"

/*
 * Simulate movement and its crystal or treasure benefit.
 *
 * An unchanged 32-bit coordinate value skips all updates. Crystal recovery clears
 * Critical; a noncritical pickup adds priority unless the policy is Retreat.
 */
void ai_simulate_movement_and_pickup(void) {
    ai_data_t* ai = &g_ai_data_base;

    if (ai->current_action.coords.word != ai->acting_unit_coords.word) {
        ai_move_temp_unit_to_coords(&ai->current_action.coords);
        if (ai->tile_pickup_kind == AI_TILE_PICKUP_CRYSTAL) {
            ai->acting_unit->hp = ai->acting_unit->max_hp;
            ai->acting_unit->mp = ai->acting_unit->max_mp;
            ai->acting_unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CRITICAL)]
                &= ~STATUS_BYTE_MASK(STATUS_ID_CRITICAL);
            ai->acting_unit->inflicted_status[STATUS_BYTE_INDEX(STATUS_ID_CRITICAL)]
                &= ~STATUS_BYTE_MASK(STATUS_ID_CRITICAL);
        }
        if (ai->tile_pickup_kind != AI_TILE_PICKUP_NONE
            && !(ai->acting_unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CRITICAL)]
                & STATUS_BYTE_MASK(STATUS_ID_CRITICAL))
            && ai->autobattle_setting != AI_AUTOBATTLE_RETREAT) {
            ai->current_action.rank.priority += 2;
        }
        ai->acting_unit->movement_taken = 1;
    }
}
