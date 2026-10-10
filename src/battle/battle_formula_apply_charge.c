#include "fft/battle.h"
#include "psx/types.h"

/* Add the Charge ability's power to XA (0 for every other action). */
void battle_formula_apply_charge(void) {
    g_battle_action_current_ability.xa += g_battle_action_current_ability.charge_power;
}
