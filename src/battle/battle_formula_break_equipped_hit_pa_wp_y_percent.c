#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x25, Head Break, Armor Break, Shield Break and Weapon Break: with no piece to break (a
 * monster, an empty slot) the action becomes a plain Attack (battle_formula_select_target_equipment);
 * otherwise the physical evade check, the hit chance PA + WP + Y with the physical modifiers
 * (battle_formula_calculate_physical_accuracy), Maintenance, and a hit breaks the piece
 * (BREAK_EQUIPMENT); a miss clears the special effects. */
void battle_formula_break_equipped_hit_pa_wp_y_percent(void) {
    battle_action_data_t* action;

    if (battle_formula_select_target_equipment() != 0) {
        g_current_ability.defaulted_to_attack = 1;
        battle_action_switch_ability_to_default_attack();
        return;
    }
    g_current_ability.defaulted_to_attack = 0;
    if (battle_formula_calculate_physical_evade() != 0) {
        return;
    }
    battle_formula_store_pa_and_weapon_power_plus_y();
    if (battle_formula_calculate_physical_accuracy() != 0) {
        /* The target writes the combined special-effect field as one halfword. */
        g_battle_action_target_data->special_effect = 0;
        return;
    }
    battle_formula_apply_maintenance();
    action = g_battle_action_target_data;
    if (action->hit != 0) {
        action->special_effect = BATTLE_ACTION_SPECIAL_EFFECT_BREAK_EQUIPMENT;
    }
}
