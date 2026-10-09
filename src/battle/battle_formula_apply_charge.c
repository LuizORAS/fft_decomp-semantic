#include "fft/battle.h"
#include "psx/types.h"

void battle_formula_apply_charge(void) {
    g_battle_action_current_ability.xa += g_battle_action_current_ability.charge_power;
}
