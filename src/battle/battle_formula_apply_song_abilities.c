#include "fft/battle_action.h"

/* Effect of each song. XA + YA is MA + Y; the stat songs add 1. */
void battle_formula_apply_song_abilities(void) {
    switch (g_action_current_ability.ability_id) {
    case ABILITY_ID_SONG_ANGEL_SONG:
        g_action_target_data->mp_healing = g_action_current_ability.xa + g_action_current_ability.ya;
        g_action_target_data->attack_type = ACTION_TYPE_MP_HEALING;
        return;
    case ABILITY_ID_SONG_LIFE_SONG:
        g_action_target_data->hp_healing = g_action_current_ability.xa + g_action_current_ability.ya;
        g_action_target_data->attack_type = ACTION_TYPE_HP_HEALING;
        return;
    case ABILITY_ID_SONG_CHEER_SONG:
        g_action_target_data->sp_change = ACTION_STAT_CHANGE_INCREASE | 1;
        break;
    case ABILITY_ID_SONG_BATTLE_SONG:
        g_action_target_data->pa_change = ACTION_STAT_CHANGE_INCREASE | 1;
        break;
    case ABILITY_ID_SONG_MAGIC_SONG:
        g_action_target_data->ma_change = ACTION_STAT_CHANGE_INCREASE | 1;
        break;
    case ABILITY_ID_SONG_NAMELESS_SONG:
        battle_formula_apply_status_to_action();
        return;
    case ABILITY_ID_SONG_LAST_SONG:
        g_action_target_data->ct_change = ACTION_CT_CHANGE_QUICK;
        break;
    }
    g_action_target_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
}
