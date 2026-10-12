#include "fft/battle_action.h"
#include "psx/types.h"

/* Lower PA (Power Break, Power Ruin), MA (Mind Break, Mind Ruin) or Speed
 * (Speed Break, Speed Ruin) by X; other abilities change nothing. */
void formula_determine_reduced_stat(void) {
    u16 ability;

    ability = g_action_current_ability.ability_id;
    if (ability == ABILITY_ID_BATTLE_SKILL_POWER_BREAK || ability == ABILITY_ID_DESTROY_SWORD_POWER_RUIN) {
        g_action_target_data->pa_change = g_action_current_ability.range_data.x & ACTION_STAT_CHANGE_VALUE_MASK;
    } else if (ability == ABILITY_ID_BATTLE_SKILL_MIND_BREAK || ability == ABILITY_ID_DESTROY_SWORD_MIND_RUIN) {
        g_action_target_data->ma_change = g_action_current_ability.range_data.x & ACTION_STAT_CHANGE_VALUE_MASK;
    } else if (ability == ABILITY_ID_BATTLE_SKILL_SPEED_BREAK || ability == ABILITY_ID_DESTROY_SWORD_SPEED_RUIN) {
        g_action_target_data->sp_change = g_action_current_ability.range_data.x & ACTION_STAT_CHANGE_VALUE_MASK;
    } else {
        return;
    }
    g_action_target_data->attack_type |= ACTION_TYPE_PSEUDO_STATUS;
}
