#include "fft/battle_action.h"

/* Formula 0x14: Golem, hit ((MA+X) * caster Faith / 100)%. */
void formula_set_golem(void) {
    if (formula_calculate_golem_accuracy() == 0) {
        /* The 16-bit store clears all other special-effect flags. */
        g_action_target_data->special_effect = ACTION_SPECIAL_EFFECT_SET_GOLEM;
        g_action_target_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
    }
}
