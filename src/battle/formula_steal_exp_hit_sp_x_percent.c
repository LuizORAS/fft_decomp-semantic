#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x28: steal (SP + Y) EXP, at most 100 and at most what the target
 * has, hit (SP+X)%; physical evade. */
void formula_steal_exp_hit_sp_x_percent(void) {
    if (formula_calculate_physical_evade() == 0) {
        formula_store_speed_and_x();
        formula_apply_attack_up_and_martial_arts();
        formula_apply_attacker_berserk_frog();
        formula_apply_defense_up();
        formula_apply_target_xa_affecting_statuses();
        formula_apply_zodiac_compatibility();
        formula_store_xa_plus_ya_status_damage();
        formula_use_hp_damage_as_action_hit_percent();
        if (g_action_target_data->hit != 0 || g_action_state != ACTION_STATE_EXECUTE) {
            formula_set_exp_stolen();
        }
    }
}
