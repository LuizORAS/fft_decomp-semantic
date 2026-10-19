#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Golem's hit chance: XA = MA, YA = X, Magic Attack Up, then XA + YA (no zodiac) scaled by the
 * caster's Faith alone (the Faith status counting as 100 and Innocent as 0) and rolled as a
 * percentage. Returns 1 on a miss. */
s32 battle_formula_calculate_golem_accuracy(void) {
    battle_action_data_t* action;

    battle_formula_store_ma_and_x();
    battle_formula_apply_magic_attack_up();
    battle_formula_store_hit_chance();
    if ((g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FAITH)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FAITH))
        != 0) {
        g_battle_action_current_ability.attacker_faith = 100;
    }
    if ((g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_INNOCENT)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_INNOCENT))
        != 0) {
        g_battle_action_current_ability.attacker_faith = 0;
    }
    action = g_battle_action_target_data;
    action->hp_damage = action->hp_damage * g_battle_action_current_ability.attacker_faith / 100;
    battle_formula_roll_hit_chance();
    return g_battle_action_target_data->hit == 0;
}
