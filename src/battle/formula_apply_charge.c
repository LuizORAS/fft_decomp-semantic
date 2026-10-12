#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Add the Charge ability's power to XA (0 for every other action). */
void formula_apply_charge(void) {
    g_action_current_ability.xa += g_action_current_ability.charge_power;
}
