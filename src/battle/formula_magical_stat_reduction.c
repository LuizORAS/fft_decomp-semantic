#include "fft/battle_action.h"

/* Formula 0x1A: -PA, -MA or -Speed (X) by ability, hit F(MA+Y)%; magic evade. */
void formula_magical_stat_reduction(void) {
    if (formula_calculate_magical_evade() == 0) {
        formula_store_ma_and_y();
        formula_apply_elemental_strengthen();
        formula_apply_magical_support_status_compatibility();
        formula_store_xa_plus_ya_status_damage();
        formula_calculate_faith();
        formula_use_hp_damage_as_action_hit_percent();
        if (g_action_target_data->hit == 0) {
            /* The target clears the bit in the special_effect 16-bit value. */
            g_action_target_data->special_effect &= ~ACTION_SPECIAL_EFFECT_BREAK_EQUIPMENT;
        } else {
            formula_determine_reduced_stat();
        }
    }
}
