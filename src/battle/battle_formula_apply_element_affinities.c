#include "fft/battle.h"

/* The target's affinities for element, on the stored HP damage: Absorb marks the action for
 * battle_formula_apply_elemental_absorption, Null nullifies it (battle_formula_nullify_action), Half
 * halves the damage, and Weak doubles it and flags the weakness. Half and Weak together cancel out. */
void battle_formula_apply_element_affinities(s32 element) {
    u8 element_mask;
    battle_stats_t* unit;
    battle_action_data_t* absorb_action;
    battle_action_data_t* half_action;
    battle_action_data_t* weak_action;

    element_mask = element;

    if (g_battle_action_target->elemental_affinity[ELEMENTAL_AFFINITY_ABSORB] & element_mask) {
        absorb_action = g_battle_action_target_data;
        /* The target updates the combined special-effect field as a halfword. */
        absorb_action->special_effect |= BATTLE_ACTION_SPECIAL_EFFECT_ELEMENTAL_ABSORPTION;
    }
    unit = g_battle_action_target;
    if (unit->elemental_affinity[ELEMENTAL_AFFINITY_NULLIFY] & element_mask) {
        battle_formula_nullify_action();
        return;
    }
    if (unit->elemental_affinity[ELEMENTAL_AFFINITY_HALF] & element_mask) {
        half_action = g_battle_action_target_data;
        half_action->hp_damage = half_action->hp_damage / 2;
    }
    if (g_battle_action_target->elemental_affinity[ELEMENTAL_AFFINITY_WEAK] & element_mask) {
        weak_action = g_battle_action_target_data;
        weak_action->special_effect |= BATTLE_ACTION_SPECIAL_EFFECT_ELEMENTAL_WEAKNESS;
        weak_action->hp_damage = weak_action->hp_damage * 2;
    }
}
