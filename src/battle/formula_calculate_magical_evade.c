#include "fft/battle_action.h"

/* Magic evade step: load the target's magic evades, then a Transparent
 * attacker, Abandon and the target's statuses; then roll. Returns 1 when the
 * attack was evaded. */
s32 formula_calculate_magical_evade(void) {
    formula_store_magical_evade_values();
    formula_calculate_transparent();
    formula_calculate_abandon();
    formula_apply_evasion_changes_due_to_statuses();
    return formula_calculate_hit();
}
