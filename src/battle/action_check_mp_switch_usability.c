#include "fft/battle_action.h"

/* MP Switch: when the target would lose HP, still has MP and passes its reaction
 * roll, move the HP damage onto MP (capped at 999) and mark the reaction.
 *
 * Only an executing action (not a preview or simulation) is converted. */
void action_check_mp_switch_usability(void) {
    unit_stats_t* unit;
    action_data_t* action;
    action_data_t* action2;

    /* The target reads hp_damage / mp_damage signed (lh). */
    if (g_action_target_data->hp_damage == 0) {
        return;
    }
    unit = g_action_target;
    if (unit->mp == 0) {
        return;
    }
    if (action_calculate_chance_to_react(g_action_target) != 0) {
        return;
    }
    if (g_action_state != ACTION_STATE_EXECUTE) {
        return;
    }
    action = g_action_target_data;
    action->mp_damage = action->mp_damage + action->hp_damage;
    if (action->mp_damage >= 0x3E8) {
        action->mp_damage = 0x3E7;
    }
    action2 = g_action_target_data;
    action2->attack_type &= ~ACTION_TYPE_HP_DAMAGE;
    action = g_action_target_data;
    action2->hp_damage = 0;
    action->attack_type |= ACTION_TYPE_MP_DAMAGE;
    g_action_target_data->reaction_id = ABILITY_ID_REACTION_MP_SWITCH;
}
