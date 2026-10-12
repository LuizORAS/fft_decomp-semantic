#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x35: heal (Y)% of max HP, hit (PA+X)%. */
void formula_heal_y_percent_hit_pa_x_percent(void) {
    formula_store_pa_and_x();
    formula_apply_attack_up_and_martial_arts();
    formula_apply_zodiac_compatibility();
    formula_store_xa_plus_ya_status_damage();
    formula_use_hp_damage_as_action_hit_percent();
    if (g_action_target_data->hit != 0 && formula_apply_status_and_check_undead() != 0) {
        formula_calculate_hp_percent_damage();
        formula_apply_undead_reversal();
    }
}
