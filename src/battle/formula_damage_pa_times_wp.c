#include "fft/battle_formula.h"

/* Formula 0x64: damage (PA * WP); with a spear (PA * 3/2 * WP); bare-handed
 * (PA * (PA * Brave / 100)). No evade roll here. */
void formula_damage_pa_times_wp(void) {
    formula_init_weapon_xa_ya();
    formula_apply_physical_status_support_compatibility();
    formula_store_xa_times_ya_damage();
}
