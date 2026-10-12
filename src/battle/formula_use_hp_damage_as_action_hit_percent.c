#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Use the percentage in hp_damage as the hit chance and clear it. 100 or more
 * always hits. Below that the shown accuracy is scaled by it and, when
 * executing, a roll of 0..99 at or above it misses; a chance of 0 misses in
 * every state. */
void formula_use_hp_damage_as_action_hit_percent(void) {
    s32 pct;
    s32 amount;
    s32 state;
    s32 xa;
    action_data_t* act;

    act = g_action_target_data;
    pct = act->hp_damage;
    if (pct >= 100) {
        act->hp_damage = 0;
        return;
    }
    xa = act->attack_accuracy;
    state = g_action_state;
    amount = pct * xa / 100;
    act->hp_damage = 0;
    act->attack_accuracy = amount;
    if (state != ACTION_STATE_EXECUTE && pct != 0) {
        return;
    }
    if (main_util_roll_pass_fail(100, pct) != 0) {
        formula_cause_action_miss();
        g_action_target_data->hp_damage = 0;
    }
}
