#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Turn absorbed elemental HP damage into HP healing; otherwise mark the action as HP damage. */
void formula_apply_elemental_absorption(void) {
    action_data_t* action = g_action_target_data;

    if (action->miss_type != ACTION_MISS_TYPE_NULLIFIED) {
        if (action->special_effect & ACTION_SPECIAL_EFFECT_ELEMENTAL_ABSORPTION) {
            u16 damage = action->hp_damage;
            action->hp_damage = 0;
            action->attack_type = ACTION_TYPE_HP_HEALING;
            action->hp_healing = damage;
        } else {
            action->attack_type = ACTION_TYPE_HP_DAMAGE;
        }
    }
}
