#include "fft/battle.h"
#include "psx/types.h"

/* Queue the weapon's spell to follow its hit (formula 0x02's 19% roll): set weapon_spell_pending,
 * put the spell (proc_id) in reaction_id and the target in post_action_target_id, and cancel the
 * knockback. The strike then continues with the spell on that target
 * (battle_action_resolve_ability_strike), unless battle_formula_cancel_weapon_spell drops it. */
void battle_formula_queue_weapon_spell(void) {
    g_current_ability.weapon_spell_pending = 1;
    g_current_ability.reaction_id = g_current_ability.proc_id;
    g_current_ability.post_action_target_id = g_current_ability.target_id;
    battle_action_cancel_knockback();
}
