#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x55, Beaking: the magical evade check and the hit chance MA + X without Faith, then PA
 * lowered by Y. */
void battle_formula_lower_pa_y(void) {
    if (battle_formula_calculate_magical_evade() != 0 || battle_formula_calculate_magic_accuracy_without_faith() != 0) {
        return;
    }
    g_battle_action_target_data->pa_change
        = g_battle_action_current_ability.range_data.y & BATTLE_ACTION_STAT_CHANGE_VALUE_MASK;
    g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
}
