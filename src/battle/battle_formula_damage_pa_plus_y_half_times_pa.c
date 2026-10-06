#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x31, Spin Fist, Wave Fist, Earth Slash, Small Bomb, Choco Ball, Turn Punch, Wave Around,
 * Sudden Cry and Snake Carrier: the physical evade check; XA = PA and YA = (PA + Y) / 2 with the
 * element's Strengthen, Attack Up and Martial Arts, the physical status modifiers and a critical hit;
 * XA * YA with the weather and the element, the absorption and the 19% status roll. */
void battle_formula_damage_pa_plus_y_half_times_pa(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        battle_formula_store_pa_and_pa_plus_y_divided_by_two();
        battle_formula_apply_ability_element_strengthen();
        battle_formula_apply_attack_up_and_martial_arts();
        battle_formula_apply_physical_status_xa_modifiers();
        battle_formula_calculate_critical_hit();
        battle_formula_store_xa_times_ya_damage();
        battle_formula_apply_weather_elemental_effects();
        battle_formula_apply_ability_element();
        if (g_battle_action_target_data->hit != 0) {
            battle_formula_apply_elemental_absorption();
            if (battle_formula_roll_conditional_status_proc() == 0) {
                battle_formula_apply_status();
            }
        }
    }
}
