#include "fft/battle.h"

/* Enter AI action simulation, save the current attacker and clear Hamedo state. */
void battle_ai_store_considered_action_data(void) {
    g_battle_action_state = BATTLE_ACTION_STATE_AI_SIMULATION;
    g_battle_action_hamedo_flag = 0;
    main_util_copy_byte_data(&g_battle_action_current_attacker, &g_battle_ai_current_attacker_backup,
        sizeof(g_battle_action_current_attacker));
}
