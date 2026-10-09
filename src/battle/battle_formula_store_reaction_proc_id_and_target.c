#include "fft/battle.h"
#include "psx/types.h"

/* Store the selected reaction and its target before clearing knockback. */
void battle_formula_store_reaction_proc_id_and_target(void) {
    g_battle_action_current_ability.weapon_proc_pending = 1;
    g_battle_action_current_ability.reaction_id = g_battle_action_current_ability.proc_id;
    g_battle_action_current_ability.post_action_target_id = g_battle_action_current_ability.target_id;
    battle_action_clear_knockback_flag();
}
