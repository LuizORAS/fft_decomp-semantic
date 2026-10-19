#include "fft/battle_action.h"

/* Arrow Guard: against an ability that uses the weapon's range with a bow or a crossbow, the accuracy
 * shown becomes 100 - Brave, and a successful Brave roll while executing turns the hit into an Arrow
 * Guard miss. */
void battle_reaction_try_arrow_guard(void) {
    battle_stats_t* target;
    battle_action_data_t* action;

    if ((g_battle_action_current_ability.range_data.flags_1 & ABILITY_SECONDARY_FLAG_1_WEAPON_RANGE) == 0) {
        return;
    }
    if (g_main_item_primary_data[g_battle_action_current_ability.weapon_id].type != ITEM_TYPE_CROSSBOW
        && g_main_item_primary_data[g_battle_action_current_ability.weapon_id].type != ITEM_TYPE_BOW) {
        return;
    }
    target = g_battle_action_target;
    action = g_battle_action_target_data;
    /* The accuracy field is written as a 16-bit value (sh) here. */
    action->attack_accuracy = 100 - target->brave;
    if (battle_reaction_fails_brave_roll(g_battle_action_target) == 0
        && g_battle_action_state == BATTLE_ACTION_STATE_EXECUTE) {
        g_battle_action_target_data->hit = 0;
        g_battle_action_target_data->miss_type = BATTLE_ACTION_MISS_TYPE_CLASS_EVADE_OR_ARROW_GUARD;
        g_battle_action_target_data->reaction_id = ABILITY_ID_REACTION_ARROW_GUARD;
    }
}
