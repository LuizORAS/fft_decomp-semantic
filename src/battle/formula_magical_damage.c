#include "fft/battle_action.h"

/* Formula 0x08: damage F(MA * Y) with element; magic evade; 19% status. */
void formula_magical_damage(void) {
    if (formula_calculate_magical_evade() == 0) {
        formula_store_ma_and_y();
        formula_apply_elemental_strengthen();
        formula_apply_magical_support_status_compatibility();
        if (formula_calculate_elemental_xa_times_ya() == 0) {
            formula_calculate_faith();
            if (formula_apply_elemental_absorption_and_status_proc() == 0) {
                formula_apply_status();
            }
        }
    }
}
