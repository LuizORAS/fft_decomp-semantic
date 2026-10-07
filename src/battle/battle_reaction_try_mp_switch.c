#include "fft/battle.h"
#include "psx/types.h"

/* MP Switch: when HP damage meets a target with MP, a successful Brave roll while executing moves the
 * whole damage to MP damage (capped at 999), even past the MP left, and HP takes none. */
void battle_reaction_try_mp_switch(void) {
    battle_stats_t* unit;
    battle_action_data_t* action;
    battle_action_data_t* action2;

    if (g_battle_action_target_data->hp_damage == 0) {
        return;
    }
    unit = g_battle_action_target;
    if (unit->mp == 0) {
        return;
    }
    if (battle_reaction_fails_brave_roll(g_battle_action_target) != 0) {
        return;
    }
    if (g_battle_action_state != BATTLE_ACTION_STATE_EXECUTE) {
        return;
    }
    action = g_battle_action_target_data;
    action->mp_damage = action->mp_damage + action->hp_damage;
    if (action->mp_damage >= 1000) {
        action->mp_damage = 999;
    }
    action2 = g_battle_action_target_data;
    action2->attack_type &= ~BATTLE_ACTION_TYPE_HP_DAMAGE;
    action = g_battle_action_target_data;
    action2->hp_damage = 0;
    action->attack_type |= BATTLE_ACTION_TYPE_MP_DAMAGE;
    g_battle_action_target_data->reaction_id = ABILITY_ID_REACTION_MP_SWITCH;
}
