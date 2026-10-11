#include "fft/battle.h"

/*
 * Simulate movement and its crystal or treasure benefit.
 *
 * An unchanged 32-bit coordinate value skips all updates. Crystal recovery clears
 * Critical; a noncritical pickup adds priority unless the policy is Retreat.
 */
void battle_ai_simulate_movement_and_pickup(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;

    if (ai->current_action.coords.word != ai->acting_unit_coords.word) {
        battle_ai_move_temp_unit_to_coords(&ai->current_action.coords);
        if (ai->tile_pickup_kind == BATTLE_AI_TILE_PICKUP_CRYSTAL) {
            ai->acting_unit->hp = ai->acting_unit->max_hp;
            ai->acting_unit->mp = ai->acting_unit->max_mp;
            ai->acting_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRITICAL)]
                &= ~BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRITICAL);
            ai->acting_unit->inflicted_status[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRITICAL)]
                &= ~BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRITICAL);
        }
        if (ai->tile_pickup_kind != BATTLE_AI_TILE_PICKUP_NONE
            && !(ai->acting_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRITICAL)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRITICAL))
            && ai->autobattle_setting != BATTLE_AI_AUTOBATTLE_RETREAT) {
            ai->current_action.rank.priority += 2;
        }
        ai->acting_unit->movement_taken = 1;
    }
}
