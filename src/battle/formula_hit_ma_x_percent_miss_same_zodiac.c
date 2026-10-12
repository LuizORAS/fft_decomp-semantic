#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x41: hit (MA+X)%; forced miss when caster and target share a zodiac sign. */
void formula_hit_ma_x_percent_miss_same_zodiac(void) {
    if (formula_calculate_magic_accuracy_without_faith() == 0) {
        if (g_action_target->birthday.zodiac == g_action_attacker->birthday.zodiac) {
            formula_force_attack_miss();
        } else {
            formula_apply_status_to_action();
        }
    }
}
