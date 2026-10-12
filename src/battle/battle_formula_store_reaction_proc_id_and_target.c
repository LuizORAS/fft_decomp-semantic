#include "fft/battle_action.h"

/* Store the selected reaction and its target before clearing knockback. */
void battle_formula_store_reaction_proc_id_and_target(void) {
    g_action_current_ability.weapon_proc_pending = 1;
    g_action_current_ability.reaction_id = g_action_current_ability.proc_id;
    g_action_current_ability.post_action_target_id = g_action_current_ability.target_id;
    action_clear_knockback_flag();
}
