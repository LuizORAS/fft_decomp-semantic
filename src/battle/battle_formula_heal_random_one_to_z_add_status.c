#include "fft/battle_action.h"
#include "fft/main.h"
#include "fft/unit.h"

/* Formula 0x4B: heal rnd(1..Z) HP once its status applies; an undead target
 * takes its current HP as damage instead. */
void battle_formula_heal_random_one_to_z_add_status(void) {
    battle_stats_t* unit;

    if (battle_formula_apply_status_and_check_undead() == 0)
        return;
    unit = g_battle_action_target;
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_UNDEAD)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_UNDEAD))
        != 0) {
        g_battle_action_target_data->hp_damage = unit->hp;
    } else {
        g_battle_action_target_data->hp_damage
            = (battle_formula_get_random_0_7fff()
                  * g_main_item_secondary_data[g_battle_action_current_ability.used_item_id].z)
                / 32768
            + 1;
    }
    battle_formula_apply_undead_reversal();
}
