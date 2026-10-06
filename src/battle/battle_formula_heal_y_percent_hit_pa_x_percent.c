#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x35, Revive and Oink: no evade check; hit chance PA + X with Attack Up and Martial Arts
 * and the zodiac; the status must change something unless the target is undead
 * (battle_formula_apply_status_and_check_undead); then Y% of max HP restored, or taken as damage by
 * an undead target. */
void battle_formula_heal_y_percent_hit_pa_x_percent(void) {
    battle_formula_store_pa_and_x();
    battle_formula_apply_attack_up_and_martial_arts();
    battle_formula_apply_zodiac_compatibility();
    battle_formula_store_hit_chance();
    battle_formula_roll_hit_chance();
    if (g_battle_action_target_data->hit != 0 && battle_formula_apply_status_and_check_undead() != 0) {
        battle_formula_calculate_hp_percent_damage();
        battle_formula_apply_undead_reversal();
    }
}
