#include "fft/battle.h"
#include "psx/types.h"

/* The Talk Skill effects on a target the talk hit: Negotiate takes gil
 * (battle_formula_calculate_stolen_gil), Persuade sets CT to 0, Praise and Threaten raise and lower
 * Brave by Y, Preach and Solution raise and lower Faith by Y; Invitation, Death Sentence, Insult and
 * Mimic Daravon add their status (battle_formula_apply_status_to_action). */
void battle_formula_apply_talk_skill(void) {
    switch (g_current_ability.ability_id) {
    case ABILITY_ID_TALK_NEGOTIATE:
        battle_formula_calculate_stolen_gil();
        return;
    case ABILITY_ID_TALK_PERSUADE:
        g_battle_action_target_data->ct_change = 0x7F;
        break;
    case ABILITY_ID_TALK_PRAISE:
        g_battle_action_target_data->brave_change = g_current_ability.range_data.y | BATTLE_ACTION_STAT_CHANGE_INCREASE;
        break;
    case ABILITY_ID_TALK_THREATEN:
        g_battle_action_target_data->brave_change
            = g_current_ability.range_data.y & BATTLE_ACTION_STAT_CHANGE_VALUE_MASK;
        break;
    case ABILITY_ID_TALK_PREACH:
        g_battle_action_target_data->faith_change = g_current_ability.range_data.y | BATTLE_ACTION_STAT_CHANGE_INCREASE;
        break;
    case ABILITY_ID_TALK_SOLUTION:
        g_battle_action_target_data->faith_change
            = g_current_ability.range_data.y & BATTLE_ACTION_STAT_CHANGE_VALUE_MASK;
        break;
    default:
        battle_formula_apply_status_to_action();
        return;
    }
    g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
}
