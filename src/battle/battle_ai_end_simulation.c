#include "fft/battle.h"

/* Leave AI simulation (battle_ai_build_command, battle_turn_run_clock): the action state goes back to
 * BATTLE_ACTION_STATE_EXECUTE and the attacker scratch saved by battle_ai_begin_simulation is restored. */
void battle_ai_end_simulation(void) {
    g_battle_action_state = BATTLE_ACTION_STATE_EXECUTE;
    main_util_copy_byte_data(
        g_battle_ai_current_ability_data_backup, &g_current_ability_attacker, sizeof(g_current_ability_attacker));
}
