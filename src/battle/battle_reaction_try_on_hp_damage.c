#include "fft/battle.h"
#include "psx/types.h"

/* PA Save, MA Save, Speed Save, Regenerator, Gilgame Heart and Auto Potion: after HP damage, a
 * successful Brave roll queues reaction_id with the damage in last_received_attack. */
void battle_reaction_try_on_hp_damage(s16 reaction_id) {
    battle_action_data_t* action;

    if ((g_battle_action_target_data->attack_type & BATTLE_ACTION_TYPE_HP_DAMAGE)
        && battle_reaction_fails_brave_roll(g_battle_action_target) == 0) {
        action = g_battle_action_target_data;
        action->reaction_id = reaction_id;
        action->last_received_attack = action->hp_damage;
    }
}
