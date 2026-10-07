#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x28, Steal Exp: the physical evade check; hit chance Speed + X with Attack Up and Martial
 * Arts, the attacker's Berserk and Frog, the target's Defense Up and statuses, and the zodiac; a hit,
 * or any estimate, takes Speed + Y EXP (battle_formula_set_exp_stolen). */
void battle_formula_steal_exp_hit_sp_x_percent(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        battle_formula_store_speed_and_x();
        battle_formula_apply_attack_up_and_martial_arts();
        battle_formula_apply_attacker_berserk_frog();
        battle_formula_apply_defense_up();
        battle_formula_apply_target_physical_status_xa_modifiers();
        battle_formula_apply_zodiac_compatibility();
        battle_formula_store_hit_chance();
        battle_formula_roll_hit_chance();
        if (g_battle_action_target_data->hit != 0 || g_battle_action_state != BATTLE_ACTION_STATE_EXECUTE) {
            battle_formula_set_exp_stolen();
        }
    }
}
