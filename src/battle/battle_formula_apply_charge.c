#include "fft/battle.h"
#include "psx/types.h"

/* Add Charge's power to XA (0 unless the command is a Charge; battle_action_run_pre_formula_setup
 * loads it from the Charge table). */
void battle_formula_apply_charge(void) {
    g_battle_action_current_ability.xa += g_battle_action_current_ability.charge_power;
}
