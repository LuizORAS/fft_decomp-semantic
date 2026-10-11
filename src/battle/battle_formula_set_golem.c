#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x14: Golem, hit ((MA+X) * caster Faith / 100)%. */
void battle_formula_set_golem(void) {
    if (battle_formula_calculate_golem_accuracy() == 0) {
        /* The 16-bit store clears all other special-effect flags. */
        g_battle_action_target_data->special_effect = BATTLE_ACTION_SPECIAL_EFFECT_SET_GOLEM;
        g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
    }
}
