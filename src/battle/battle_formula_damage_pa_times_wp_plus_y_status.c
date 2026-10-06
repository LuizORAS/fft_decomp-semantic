#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x2D, Holy Sword (Stasis Sword, Split Punch, Crush Punch, Lightning Stab, Holy
 * Explosion): the physical evade check, XA = PA and YA = WP + Y with the weapon element's Strengthen,
 * the physical damage, the target's affinities for the weapon's element and the absorption, then the
 * ability's status on every hit (battle_formula_apply_status). The ability's own element is not
 * read (QUIRKS.md). */
void battle_formula_damage_pa_times_wp_plus_y_status(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        battle_formula_store_pa_and_weapon_power_plus_y();
        battle_formula_apply_weapon_element_strengthen();
        battle_formula_calculate_physical_damage();
        battle_formula_apply_weapon_element();
        if (g_battle_action_target_data->hit != 0) {
            battle_formula_apply_elemental_absorption();
            battle_formula_apply_status();
        }
    }
}
