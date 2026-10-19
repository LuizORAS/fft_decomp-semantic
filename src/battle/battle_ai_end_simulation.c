#include "fft/battle_action.h"
#include "fft/battle_ai.h"
#include "fft/memory.h"

/* Leave AI simulation (battle_ai_build_command, battle_turn_run_clock): the action state goes back to
 * BATTLE_ACTION_STATE_EXECUTE and the attacker scratch saved by battle_ai_begin_simulation is restored. */
void battle_ai_end_simulation(void) {
    g_battle_action_state = BATTLE_ACTION_STATE_EXECUTE;
    main_mem_copy_byte_data_source_first(&g_battle_ai_current_attacker_backup, &g_battle_action_current_attacker,
        sizeof(g_battle_action_current_attacker));
}
