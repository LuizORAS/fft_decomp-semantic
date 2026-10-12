#include "fft/battle_action.h"
#include "fft/main.h"

/* Formula 0x48: heal (Z * 10) HP. */
void formula_heal_z_times_ten(void) {
    g_action_target_data->hp_damage = g_main_item_secondary_data[g_action_current_ability.used_item_id].z * 10;
    formula_apply_undead_reversal();
}
