#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "fft/main.h"
#include "fft/unit.h"

/* Formula 0x4B: heal rnd(1..Z) HP once its status applies; an undead target
 * takes its current HP as damage instead. */
void formula_heal_random_one_to_z_add_status(void) {
    battle_stats_t* unit;

    if (formula_apply_status_and_check_undead() == 0)
        return;
    unit = g_action_target;
    if ((unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_UNDEAD)] & STATUS_BYTE_MASK(STATUS_ID_UNDEAD)) != 0) {
        g_action_target_data->hp_damage = unit->hp;
    } else {
        g_action_target_data->hp_damage
            = (formula_get_random_0_7fff() * g_main_item_secondary_data[g_action_current_ability.used_item_id].z)
                / 32768
            + 1;
    }
    formula_apply_undead_reversal();
}
