#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x14, Golem: hit chance from the caster's MA + X and Faith
 * (battle_formula_calculate_golem_accuracy), then the SET_GOLEM special effect, which
 * battle_action_apply_target_result turns into the Golem guard. */
void battle_formula_set_golem(void) {
    if (battle_formula_calculate_golem_accuracy() == 0) {
        /* The halfword store clears all other special-effect flags. */
        g_battle_action_target_data->special_effect = BATTLE_ACTION_SPECIAL_EFFECT_SET_GOLEM;
        g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
    }
}
