#include "fft/battle.h"

/* Physical damage from XA and YA: the physical XA modifiers
 * (battle_formula_apply_physical_xa_modifiers), a critical hit, then HP damage XA * YA. */
void battle_formula_calculate_physical_damage(void) {
    battle_formula_apply_physical_xa_modifiers();
    battle_formula_calculate_critical_hit();
    battle_formula_store_xa_times_ya_damage();
}
