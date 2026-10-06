#include "fft/battle.h"

/* The magical evade check: the target's magic evades (battle_formula_store_magical_evade_values, no
 * class evade), the attacker's Transparent, the target's Abandon and statuses, then the rolls
 * (battle_formula_roll_evades); the attack direction does not count. Returns 1 when an evade blocked
 * the spell. */
s32 battle_formula_calculate_magical_evade(void) {
    battle_formula_store_magical_evade_values();
    battle_formula_calculate_transparent();
    battle_formula_calculate_abandon();
    battle_formula_apply_evasion_changes_due_to_statuses();
    return battle_formula_roll_evades();
}
