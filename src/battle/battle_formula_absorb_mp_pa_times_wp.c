#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x2F, Dark Sword: the physical evade check, then PA * WP as physical damage drained as MP
 * (battle_formula_apply_mp_absorption). */
void battle_formula_absorb_mp_pa_times_wp(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        battle_formula_store_pa_and_weapon_power();
        battle_formula_calculate_physical_damage();
        battle_formula_apply_mp_absorption();
    }
}
