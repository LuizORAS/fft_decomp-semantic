#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x1A, Speed Ruin, Power Ruin and Mind Ruin: the magical evade check; hit chance MA + Y with
 * the element's Strengthen, the magical XA modifiers and both Faiths; a hit lowers Speed, PA or MA by
 * X (battle_formula_determine_reduced_stat), a miss clears the BREAK_EQUIPMENT flag. */
void battle_formula_lower_stat_x_hit_faith_ma_y_percent(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        battle_formula_store_ma_and_y();
        battle_formula_apply_ability_element_strengthen();
        battle_formula_apply_magical_xa_modifiers();
        battle_formula_store_hit_chance();
        battle_formula_calculate_faith();
        battle_formula_roll_hit_chance();
        if (g_battle_action_target_data->hit == 0) {
            /* The target clears the bit in the special_effect halfword. */
            g_battle_action_target_data->special_effect &= ~BATTLE_ACTION_SPECIAL_EFFECT_BREAK_EQUIPMENT;
        } else {
            battle_formula_determine_reduced_stat();
        }
    }
}
