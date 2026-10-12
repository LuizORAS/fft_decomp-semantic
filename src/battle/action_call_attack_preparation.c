#include "fft/battle_action.h"

/* Validate the action in place with action_prepare_attack phase 0, which
 * consumes nothing and leaves the unit's turn flags alone. */
void action_call_attack_preparation(action_command_t* action) {
    action_prepare_attack(action, action, 0);
}
