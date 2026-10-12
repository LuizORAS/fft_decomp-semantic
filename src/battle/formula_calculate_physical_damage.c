#include "fft/battle_formula.h"

/* Physical damage: XA * YA after the physical modifiers and the critical roll. */
void formula_calculate_physical_damage(void) {
    formula_apply_physical_xa_modifying_statuses_support();
    formula_calculate_critical_hit();
    formula_store_xa_times_ya_damage();
}
