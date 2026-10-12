#include "fft/battle_action.h"
#include "fft/battle_ai.h"

/* End an AI simulation: copy the saved current-attacker block back and return
 * the action state to execute. */
void battle_ai_restore_considered_action_data(void) {
    g_battle_action_state = BATTLE_ACTION_STATE_EXECUTE;
    main_util_copy_byte_data(&g_battle_ai_current_attacker_backup, &g_battle_action_current_attacker,
        sizeof(g_battle_action_current_attacker));
}
