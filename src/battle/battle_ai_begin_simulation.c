#include "fft/battle.h"

/* Enter AI simulation before planning (battle_ai_build_command): the action state becomes
 * BATTLE_ACTION_STATE_AI_SIMULATION (no graphics are queued and only Reflect reacts), a pending First
 * Strike cancel is cleared, and the current ability's attacker scratch (g_battle_action_current_attacker, 30
 * bytes) is saved. */
void battle_ai_begin_simulation(void) {
    g_battle_action_state = BATTLE_ACTION_STATE_AI_SIMULATION;
    g_battle_reaction_hamedo_cancels = 0;
    main_util_copy_byte_data(&g_battle_action_current_attacker, g_battle_ai_current_ability_data_backup,
        sizeof(g_battle_action_current_attacker));
}
