#include "fft/battle_action.h"
#include "psx/types.h"

/* Record reaction_id when the action deals HP damage and the target passes its
 * reaction roll; the HP damage is the reaction's amount. */
void action_check_pa_save_ma_save_speed_save_regenerator_auto_potion_gilgame_heart_usability(s16 reaction_id) {
    action_data_t* action;

    if ((g_action_target_data->attack_type & ACTION_TYPE_HP_DAMAGE)
        && action_calculate_chance_to_react(g_action_target) == 0) {
        action = g_action_target_data;
        action->reaction_id = reaction_id;
        action->last_received_attack = action->hp_damage;
    }
}
