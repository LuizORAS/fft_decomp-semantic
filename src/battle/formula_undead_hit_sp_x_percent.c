#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "fft/unit.h"

/* Formula 0x40: undead targets only, hit (SP+X)%. */
void formula_undead_hit_sp_x_percent(void) {
    formula_store_speed_and_x();
    formula_apply_physical_xa_modifying_statuses_support();
    formula_store_xa_plus_ya_status_damage();
    formula_use_hp_damage_as_action_hit_percent();
    if (g_action_target_data->hit != 0) {
        if ((g_action_target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_UNDEAD)]
                & STATUS_BYTE_MASK(STATUS_ID_UNDEAD))
            == 0) {
            formula_force_attack_miss();
        } else {
            formula_apply_status_to_action();
        }
    }
}
