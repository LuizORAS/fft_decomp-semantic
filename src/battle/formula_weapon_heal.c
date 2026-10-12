#include "fft/battle_action.h"

/* Formula 0x07: heal weapon (XA * YA) HP (damage to undead); no evade. Zodiac
 * compatibility is applied twice. */
void formula_weapon_heal(void) {
    formula_calculate_base_xa();
    formula_apply_zodiac_compatibility();
    formula_apply_charge();
    formula_apply_physical_xa_modifying_statuses_support();
    formula_store_xa_times_ya_damage();
    formula_apply_undead_reversal();
}
