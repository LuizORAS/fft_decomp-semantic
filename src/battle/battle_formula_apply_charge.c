#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Add Charge's power to XA (0 unless the command is a Charge; battle_action_run_pre_formula_setup
 * loads it from the Charge table). */
void battle_formula_apply_charge(void) {
    g_battle_action_current_ability.xa += g_battle_action_current_ability.charge_power;
}
