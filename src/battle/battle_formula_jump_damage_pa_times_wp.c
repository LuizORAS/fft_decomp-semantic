#include "fft/battle.h"

/* Formula 0x64, the Jump command (battle_action_run_pre_formula_setup selects it): XA and YA for
 * Jump (battle_formula_store_jump_xa_ya: PA, or PA * 3 / 2 with a spear, times the weapon's power,
 * or bare-handed times PA * Brave / 100), the physical status modifiers, then XA * YA. No evade
 * check, critical hit, element or status. */
void battle_formula_jump_damage_pa_times_wp(void) {
    battle_formula_store_jump_xa_ya();
    battle_formula_apply_physical_status_xa_modifiers();
    battle_formula_store_xa_times_ya_damage();
}
