#include "fft/battle.h"

/* START_ACTION_EXECUTE: one frame after the attack animation is chosen, gather the targets,
 * play the effect and enter ACTION_EXECUTE (battle_action_play_ability_effect). */
void battle_state_handle_start_action_execute_state(void) {
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_casting_misc_data();
    unit->state_frame_counter++;
    battle_action_play_ability_effect();
    battle_state_update_units();
}
