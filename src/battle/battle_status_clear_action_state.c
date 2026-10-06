#include "fft/battle.h"
#include "psx/types.h"

/* Clear the unit's action state (charging, jumping, defending, performing) through
 * main_status_set_action_state. A new command clears it (battle_action_commit_command and the menu
 * and AI command states). battle_status_clear_action_state_2 is a byte-identical twin. */
void battle_status_clear_action_state(battle_stats_t* unit) {
    main_status_set_action_state(unit, MAIN_UNIT_ACTION_STATE_NONE);
}
