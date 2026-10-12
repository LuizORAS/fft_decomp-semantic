#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x26: steal equipped item, hit (SP+X)%. */
void formula_steal_equipment(void) {
    action_data_t* action;

    if (formula_apply_steal_break_might_sword_hardcoding() != 0) {
        formula_nullify_action();
        g_action_target_data->miss_type = ACTION_MISS_TYPE_FORCED_FAILURE;
        return;
    }
    if (formula_calculate_physical_evade() != 0) {
        return;
    }
    formula_store_speed_and_x();
    if (formula_calculate_physical_status_accuracy() != 0) {
        return;
    }
    formula_apply_maintenance();
    action = g_action_target_data;
    if (action->hit != 0) {
        /* The 16-bit store clears all other special-effect flags. */
        action->special_effect = ACTION_SPECIAL_EFFECT_STEAL_ITEM;
    }
}
