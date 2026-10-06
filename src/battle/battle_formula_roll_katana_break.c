#include "fft/battle.h"
#include "psx/types.h"

/* Draw Out's katana, on the attacker's own result (hit, the katana as item_lost): while executing, a
 * 0-99 roll below the katana's power (WP) breaks it (DRAW_OUT_KATANA_BROKEN), otherwise it stays
 * (DRAW_OUT_KATANA_NOT_BROKEN); an estimate always records it broken. A Mime uses no katana and skips
 * the roll. */
void battle_formula_roll_katana_break(void) {
    if (g_battle_action_attacker->job_id == JOB_ID_MIME) {
        return;
    }

    g_battle_action_attacker_data->hit = 1;
    g_battle_action_attacker_data->item_lost = g_battle_action_attacker->used_item_or_equipment;
    g_battle_action_attacker_data->miss_type = BATTLE_ACTION_MISS_TYPE_CANCELLED;

    if (g_battle_action_state == BATTLE_ACTION_STATE_EXECUTE
        && main_util_roll_pass_fail(100, g_current_ability.weapon_data.power) != 0) {
        g_battle_action_attacker_data->special_effect = BATTLE_ACTION_SPECIAL_EFFECT_DRAW_OUT_KATANA_NOT_BROKEN;
    } else {
        g_battle_action_attacker_data->special_effect = BATTLE_ACTION_SPECIAL_EFFECT_DRAW_OUT_KATANA_BROKEN;
    }
}
