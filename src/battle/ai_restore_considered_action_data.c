#include "fft/battle_action.h"
#include "fft/battle_ai.h"

/* End an AI simulation: copy the saved current-attacker block back and return
 * the action state to execute. */
void ai_restore_considered_action_data(void) {
    g_action_state = ACTION_STATE_EXECUTE;
    main_util_copy_byte_data(
        &g_ai_current_attacker_backup, &g_action_current_attacker, sizeof(g_action_current_attacker));
}
