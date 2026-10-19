#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "fft/unit.h"

/* Formula 0x40, Seal Evil: formula 0x3F's hit chance, but a target that is not undead fails (forced
 * failure). */
void battle_formula_undead_hit_sp_x_percent(void) {
    battle_formula_store_speed_and_x();
    battle_formula_apply_physical_xa_modifiers();
    battle_formula_store_hit_chance();
    battle_formula_roll_hit_chance();
    if (g_battle_action_target_data->hit != 0) {
        if ((g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_UNDEAD)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_UNDEAD))
            == 0) {
            battle_formula_force_attack_miss();
        } else {
            battle_formula_apply_status_to_action();
        }
    }
}
