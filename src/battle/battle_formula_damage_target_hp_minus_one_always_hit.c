#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x3E: damage equal to the target's current HP - 1 (0 when it has none). */
void battle_formula_damage_target_hp_minus_one_always_hit(void) {
    battle_formula_calculate_gravi2_damage();
}
