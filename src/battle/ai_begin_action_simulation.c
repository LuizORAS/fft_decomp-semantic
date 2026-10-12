#include "fft/battle_action.h"
#include "fft/battle_ai.h"

/* Enter AI action simulation, save the current attacker and clear Hamedo state. */
void ai_begin_action_simulation(void) {
    g_action_state = ACTION_STATE_AI_SIMULATION;
    g_action_hamedo_flag = 0;
    main_util_copy_byte_data(
        &g_action_current_attacker, &g_ai_current_attacker_backup, sizeof(g_action_current_attacker));
}
