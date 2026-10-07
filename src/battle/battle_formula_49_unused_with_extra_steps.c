#include "fft/battle.h"
#include "psx/types.h"

/* Unreferenced: it only calls battle_formula_heal_mp_z_times_ten (formula 0x49); no table entry,
 * call or data word in any module points to it. */
void battle_formula_49_unused_with_extra_steps(void) {
    battle_formula_heal_mp_z_times_ten();
}
