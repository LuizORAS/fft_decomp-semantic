#include "fft/battle_formula.h"

/* Physical evade step: load the target's evades, then Concentrate, a Dark,
 * Confused or Transparent attacker, Abandon, the target's statuses, the bow
 * weather penalty and facing; then roll. Returns 1 when the attack was evaded. */
s32 formula_calculate_physical_evade(void) {
    formula_store_physical_evade_values();
    formula_calculate_concentrate();
    formula_calculate_dark_confuse();
    formula_calculate_abandon();
    formula_apply_evasion_changes_due_to_statuses();
    formula_apply_weather_effects_on_bows();
    formula_calculate_facing_evade();
    return formula_calculate_hit();
}
