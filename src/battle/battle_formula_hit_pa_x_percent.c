#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x33, Stigma Magic: no evade check; hit chance PA + X with Attack Up and Martial Arts, the
 * attacker's Berserk and Frog and the zodiac, then the ability's status
 * (battle_formula_apply_status_to_action). */
void battle_formula_hit_pa_x_percent(void) {
    battle_formula_store_pa_and_x();
    battle_formula_apply_attack_up_and_martial_arts();
    battle_formula_apply_attacker_berserk_frog();
    battle_formula_apply_zodiac_compatibility();
    battle_formula_store_hit_chance();
    battle_formula_roll_hit_chance();
    if (g_battle_action_target_data->hit != 0) {
        battle_formula_apply_status_to_action();
    }
}
