#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x48, Potion, Hi-Potion and X-Potion: restore the item's Z * 10 HP (30, 70, 150), or deal
 * it to an undead target (battle_formula_apply_undead_reversal). */
void battle_formula_heal_z_times_ten(void) {
    g_battle_action_target_data->hp_damage = g_main_item_secondary_data[g_current_ability.used_item_id].z * 10;
    battle_formula_apply_undead_reversal();
}
