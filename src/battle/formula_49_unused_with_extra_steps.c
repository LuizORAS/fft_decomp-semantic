#include "fft/battle_action.h"

/* A second entry to formula 0x49's handler. The formula table does not point
 * here and no retail code calls it. */
void formula_49_unused_with_extra_steps(void) {
    formula_heal_mp_z_times_ten();
}
