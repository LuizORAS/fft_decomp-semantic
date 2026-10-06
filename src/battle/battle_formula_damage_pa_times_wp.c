#include "fft/battle.h"

/* Formula 0x64: damage (PA * WP); spear (PA * 3/2 * WP); weaponless (PA * Brave/100 * WP). */
void battle_formula_damage_pa_times_wp(void) {
    battle_formula_store_jump_xa_ya();
    battle_formula_apply_physical_status_xa_modifiers();
    battle_formula_store_xa_times_ya_damage();
}
