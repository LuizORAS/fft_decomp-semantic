#include "fft/battle.h"

/* Move the simulation unit to the current action's destination, if it differs. A crystal there
 * restores its HP and MP and clears Critical; a crystal or treasure adds 2 to the score unless the
 * unit is still Critical or its Auto-Battle mode is Run like a rabbit. The unit counts as moved. */
void battle_ai_simulate_movement_and_pickup(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;

    if (ai->current_action.coords.word != ai->acting_unit_coords.word) {
        battle_ai_move_temp_unit_to_coords(&ai->current_action.coords);
        if (ai->crystal_treasure_status == 1) {
            ai->acting_unit->hp = ai->acting_unit->max_hp;
            ai->acting_unit->mp = ai->acting_unit->max_mp;
            ai->acting_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRITICAL)]
                &= ~BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRITICAL);
            ai->acting_unit->inflicted_status[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRITICAL)]
                &= ~BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRITICAL);
        }
        if (ai->crystal_treasure_status != 0
            && !(ai->acting_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRITICAL)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRITICAL))
            && ai->autobattle_setting != BATTLE_AUTO_BATTLE_RUN_LIKE_A_RABBIT) {
            ai->current_action.rank.priority += 2;
        }
        ai->acting_unit->movement_taken = 1;
    }
}
