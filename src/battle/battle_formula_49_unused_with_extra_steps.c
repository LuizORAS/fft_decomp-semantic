#include "fft/battle.h"
#include "psx/types.h"

/* A second entry to formula 0x49's handler. The formula table does not point
 * here and no retail code calls it. */
void battle_formula_49_unused_with_extra_steps(void) {
    battle_formula_heal_mp_z_times_ten();
}
