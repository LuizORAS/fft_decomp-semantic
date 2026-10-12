#include "fft/battle_action.h"
#include "psx/types.h"

/* Formula 0x03: gun damage (WP * WP); no evade roll, element or status. */
void formula_gun_damage(void) {
    u16 weapon_power = g_action_current_ability.weapon_data.power;

    g_action_current_ability.xa = weapon_power;
    g_action_current_ability.ya = weapon_power;
    formula_apply_charge();
    formula_calculate_physical_damage();
}
