#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x39, Yell: raise Speed by Y; no hit roll. */
void battle_formula_add_speed_y(void) {
    g_battle_action_target_data->sp_change = g_current_ability.range_data.y | BATTLE_ACTION_STAT_CHANGE_INCREASE;
    g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
}
