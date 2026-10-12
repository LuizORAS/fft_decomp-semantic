#include "fft/battle_action.h"

/* Formula 0x39: +Speed (Y). */
void battle_formula_add_speed_y(void) {
    g_action_target_data->sp_change = g_action_current_ability.range_data.y | ACTION_STAT_CHANGE_INCREASE;
    g_action_target_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
}
