#include "fft/battle_action.h"

/* Add the Charge ability's power to XA (0 for every other action). */
void battle_formula_apply_charge(void) {
    g_action_current_ability.xa += g_action_current_ability.charge_power;
}
