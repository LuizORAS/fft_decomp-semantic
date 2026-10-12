#include "fft/battle_action.h"

/* Applies Arrow Guard to bow and crossbow attacks that use weapon range.
 *
 * A successful Brave-based reaction changes the target result to the shared
 * class-evade/Arrow Guard miss type. */
void action_check_arrow_guard_usability(void) {
    battle_stats_t* target;
    action_data_t* action;

    if ((g_action_current_ability.range_data.flags_1 & ABILITY_SECONDARY_FLAG_1_WEAPON_RANGE) == 0) {
        return;
    }
    if (g_main_item_primary_data[g_action_current_ability.weapon_id].type != ITEM_TYPE_CROSSBOW
        && g_main_item_primary_data[g_action_current_ability.weapon_id].type != ITEM_TYPE_BOW) {
        return;
    }
    target = g_action_target;
    action = g_action_target_data;
    /* The accuracy field is written as a 16-bit value (sh) here. */
    action->attack_accuracy = 100 - target->brave;
    if (action_calculate_chance_to_react(g_action_target) == 0 && g_action_state == ACTION_STATE_EXECUTE) {
        g_action_target_data->hit = 0;
        g_action_target_data->miss_type = ACTION_MISS_TYPE_CLASS_EVADE_OR_ARROW_GUARD;
        g_action_target_data->reaction_id = ABILITY_ID_REACTION_ARROW_GUARD;
    }
}
