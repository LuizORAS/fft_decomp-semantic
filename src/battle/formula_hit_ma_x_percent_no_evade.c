#include "fft/battle_action.h"

/* Formula 0x51: hit (MA+X)%. */
void formula_hit_ma_x_percent_no_evade(void) {
    formula_store_ma_and_x();
    formula_apply_elemental_strengthen();
    formula_apply_magic_attack_up();
    formula_apply_zodiac_compatibility();
    formula_store_xa_plus_ya_status_damage();
    formula_use_hp_damage_as_action_hit_percent();
    if (g_action_target_data->hit != 0) {
        formula_apply_status_to_action();
    }
}
