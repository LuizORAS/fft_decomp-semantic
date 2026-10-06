#include "fft/battle.h"
#include "psx/types.h"

/* Damage Split: after HP damage, a successful Brave roll queues the reaction with half the damage,
 * rounding up, in last_received_attack. */
void battle_reaction_try_damage_split(void) {
    u16 hp;
    battle_action_data_t* action;

    hp = g_battle_action_target_data->hp_damage;
    if (hp != 0 && battle_reaction_fails_brave_roll(g_battle_action_target) == 0) {
        hp++;
        hp = ((u16)hp) >> 1;
        action = g_battle_action_target_data;
        action->last_received_attack = hp;
        action->reaction_id = ABILITY_ID_REACTION_DAMAGE_SPLIT;
    }
}
