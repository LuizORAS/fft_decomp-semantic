#include "fft/battle_action.h"

/* Formula 0x33: hit (PA+X)%. */
void formula_hit_pa_x_percent(void) {
    formula_store_pa_and_x();
    formula_apply_attack_up_and_martial_arts();
    formula_apply_attacker_berserk_frog();
    formula_apply_zodiac_compatibility();
    formula_store_xa_plus_ya_status_damage();
    formula_use_hp_damage_as_action_hit_percent();
    if (g_action_target_data->hit != 0) {
        formula_apply_status_to_action();
    }
}
