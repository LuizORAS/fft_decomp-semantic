#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x0F: absorb (Y)% of max MP, hit F(MA+X)%. */
void formula_absorb_mp_y_percent(void) {
    if (formula_calculate_magical_evade() != 0) {
        return;
    }
    if (formula_calculate_magic_accuracy_no_elemental() != 0) {
        return;
    }
    formula_calculate_mp_percent_damage();
    g_action_target_data->hp_damage = g_action_target_data->mp_damage;
    formula_apply_mp_recovery();
}
