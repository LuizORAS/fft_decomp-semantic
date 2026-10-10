#include "fft/battle.h"

/* Magic evade step: load the target's magic evades, then a Transparent
 * attacker, Abandon and the target's statuses; then roll. Returns 1 when the
 * attack was evaded. */
s32 battle_formula_calculate_magical_evade(void) {
    battle_formula_store_magical_evade_values();
    battle_formula_calculate_transparent();
    battle_formula_calculate_abandon();
    battle_formula_apply_evasion_changes_due_to_statuses();
    return battle_formula_calculate_hit();
}
