#include "fft/battle.h"
#include "psx/types.h"

/* Give the target its reactions that come before the formula, in a primary action: Reflect when it
 * has the status and the strike carries no reaction yet; then, when it can still react and act, Blade
 * Grasp or else Arrow Guard. Formula 7 and a strike that already carries a reaction skip them. */
void battle_reaction_check_before_formula(void) {
    u8 reaction_flags;

    if (g_battle_action_context != BATTLE_ACTION_CONTEXT_PRIMARY) {
        return;
    }
    if (g_current_ability.formula == BATTLE_FORMULA_WEAPON_HEAL) {
        return;
    }
    if (battle_reaction_get_pending(g_battle_action_target) == 1) {
        return;
    }
    if ((g_battle_action_target->status_sets.current[4] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_REFLECT))
        && g_current_ability.reaction_id == 0) {
        battle_reaction_mark_reflected();
    }
    if (g_battle_action_target_data->reaction_id != 0) {
        return;
    }
    if (battle_reaction_is_prevented(g_battle_action_target) != 0) {
        return;
    }
    if (battle_unit_get_action_block(g_battle_action_target) != 0) {
        return;
    }
    reaction_flags = g_battle_action_target->reaction_abilities[3];
    if (reaction_flags & BATTLE_REACTION_SET_4_BLADE_GRASP) {
        battle_reaction_try_blade_grasp();
        return;
    }
    if (reaction_flags & BATTLE_REACTION_SET_4_ARROW_GUARD) {
        battle_reaction_try_arrow_guard();
    }
}
