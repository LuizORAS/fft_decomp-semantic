#include "fft/battle.h"

/* Formula 0x41: hit (MA+X)%; forced miss when caster and target share a zodiac sign. */
void battle_formula_hit_ma_x_percent_miss_same_zodiac(void) {
    if (battle_formula_calculate_magic_accuracy_without_faith() == 0) {
        if (g_battle_action_target->birthday.zodiac == g_battle_action_attacker->birthday.zodiac) {
            battle_formula_force_attack_miss();
        } else {
            battle_formula_apply_status_to_action();
        }
    }
}
