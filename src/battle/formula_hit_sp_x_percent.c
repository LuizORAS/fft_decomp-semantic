#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x3F: hit (SP+X)%. */
void formula_hit_sp_x_percent(void) {
    formula_store_speed_and_x();
    formula_apply_physical_xa_modifying_statuses_support();
    formula_store_xa_plus_ya_status_damage();
    formula_use_hp_damage_as_action_hit_percent();
    if (g_action_target_data->hit != 0) {
        formula_apply_status_to_action();
    }
}
