#include "fft/battle.h"
#include "psx/types.h"

/* Queue the weapon's spell when it replaced the weapon's hit (battle_formula_weapon_damage_with_proc):
 * set weapon_spell_pending, put the spell (proc_id) in reaction_id and the target in
 * post_action_target_id, and cancel the knockback. The next strike casts it
 * (battle_action_resolve_ability_strike). */
void battle_formula_queue_weapon_spell(void) {
    g_current_ability.weapon_spell_pending = 1;
    g_current_ability.reaction_id = g_current_ability.proc_id;
    g_current_ability.post_action_target_id = g_current_ability.target_id;
    battle_action_cancel_knockback();
}
