#include "fft/battle.h"

/* Validate the action in place with battle_action_prepare_attack phase 0, which
 * consumes nothing and leaves the unit's turn flags alone. */
void battle_action_call_attack_preparation(battle_ai_command_action_t* action) {
    battle_action_prepare_attack(action, action, 0);
}
