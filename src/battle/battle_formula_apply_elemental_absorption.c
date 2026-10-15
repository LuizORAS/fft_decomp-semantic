#include "fft/battle.h"
#include "psx/types.h"

/* Turn the HP damage into HP healing when an affinity absorbed the element
 * (battle_formula_apply_element_affinities); otherwise mark the action as HP damage. A nullified
 * action is left alone. */
void battle_formula_apply_elemental_absorption(void) {
    battle_action_data_t* action = g_battle_action_target_data;

    if (action->miss_type != BATTLE_ACTION_MISS_TYPE_NULLIFIED) {
        if (action->special_effect & BATTLE_ACTION_SPECIAL_EFFECT_ELEMENTAL_ABSORPTION) {
            u16 damage = action->hp_damage;
            action->hp_damage = 0;
            action->attack_type = BATTLE_ACTION_TYPE_HP_HEALING;
            action->hp_healing = damage;
        } else {
            action->attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE;
        }
    }
}
