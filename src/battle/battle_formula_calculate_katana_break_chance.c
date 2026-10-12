#include "fft/battle_action.h"

/* Draw Out: the used katana breaks when a roll of 0..99 is below its weapon
 * power. The roll is made only when executing; any other state records it as
 * broken. A Mime's katana is not touched. */
void battle_formula_calculate_katana_break_chance(void) {
    if (g_battle_action_attacker->job_id == JOB_ID_MIME) {
        return;
    }

    g_battle_action_attacker_data->hit = 1;
    g_battle_action_attacker_data->item_lost = g_battle_action_attacker->action_command.item_or_equipment;
    g_battle_action_attacker_data->miss_type = BATTLE_ACTION_MISS_TYPE_CANCELLED;

    if (g_battle_action_state == BATTLE_ACTION_STATE_EXECUTE
        && main_util_roll_pass_fail(100, g_battle_action_current_ability.weapon_data.power) != 0) {
        g_battle_action_attacker_data->special_effect = BATTLE_ACTION_SPECIAL_EFFECT_DRAW_OUT_KATANA_NOT_BROKEN;
    } else {
        g_battle_action_attacker_data->special_effect = BATTLE_ACTION_SPECIAL_EFFECT_DRAW_OUT_KATANA_BROKEN;
    }
}
