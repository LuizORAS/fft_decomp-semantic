#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x03, the guns (Romanda Gun, Mythril Gun, Stone Gun): XA = YA = the gun's power, Charge's
 * power, then the physical damage (battle_formula_calculate_physical_damage), so WP * WP with the
 * physical modifiers and a critical hit. There is no evade check, so guns ignore evasion, and
 * battle_action_run_pre_formula_setup loads no status for formula 3. */
void battle_formula_gun_damage(void) {
    u16 weapon_power = g_current_ability.weapon_data.power;

    g_current_ability.xa = weapon_power;
    g_current_ability.ya = weapon_power;
    battle_formula_apply_charge();
    battle_formula_calculate_physical_damage();
}
