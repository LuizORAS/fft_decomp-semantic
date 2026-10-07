#include "fft/battle.h"
#include "psx/types.h"

/* Roll the hit chance that the accuracy steps left in hp_damage: 100 or more always hits; otherwise
 * it scales the shown accuracy and, while executing, a roll of 0-99 at or above it misses
 * (battle_formula_set_accuracy_miss); 0 always misses, even in an estimate. hp_damage is cleared
 * either way. */
void battle_formula_roll_hit_chance(void) {
    s32 pct;
    s32 amount;
    s32 state;
    s32 xa;
    battle_action_data_t* act;

    act = g_battle_action_target_data;
    pct = act->hp_damage;
    if (pct >= 100) {
        act->hp_damage = 0;
        return;
    }
    xa = act->attack_accuracy;
    state = g_battle_action_state;
    amount = pct * xa / 100;
    act->hp_damage = 0;
    act->attack_accuracy = amount;
    if (state != BATTLE_ACTION_STATE_EXECUTE && pct != 0) {
        return;
    }
    if (main_util_roll_pass_fail(100, pct) != 0) {
        battle_formula_set_accuracy_miss();
        g_battle_action_target_data->hp_damage = 0;
    }
}
