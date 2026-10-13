#include "fft/battle_action.h"
#include "fft/battle_ai.h"
#include "fft/memory.h"

/* End an AI simulation: copy the saved current-attacker block back and return
 * the action state to execute. */
void ai_end_action_simulation(void) {
    g_action_state = ACTION_STATE_EXECUTE;
    mem_copy_byte_data_source_first(
        &g_ai_current_attacker_backup, &g_action_current_attacker, sizeof(g_action_current_attacker));
}
