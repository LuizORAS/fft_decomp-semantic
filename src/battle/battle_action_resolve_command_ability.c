#include "fft/battle_action.h"

/* Resolve the ability a command uses, in place (battle_action_prepare_attack, phase 0): the action
 * menus and the AI command learn the ability id this way before targeting, and nothing on the unit
 * changes. */
void battle_action_resolve_command_ability(action_command_t* action) {
    battle_action_prepare_attack(action, action, 0);
}
