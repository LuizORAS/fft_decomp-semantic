#include "fft/battle.h"
#include "psx/types.h"

/* Arm a counter-type reaction on the target: the attack must not come through
 * Math Skill, its flags_4 must have a bit of mask, and the target's Brave roll
 * must pass. Records the reaction and the ability it answers. */
void battle_action_check_counter_reaction_usability(u16 reaction_id, u32 mask) {
    battle_action_data_t* action;

    if (g_battle_action_current_ability.skillset == SKILLSET_ID_MATH_SKILL)
        return;
    if ((g_battle_action_current_ability.range_data.flags_4 & mask) == 0)
        return;
    if (battle_action_calculate_chance_to_react(g_battle_action_target) != 0)
        return;
    action = g_battle_action_target_data;
    action->reaction_id = reaction_id;
    action->last_received_attack = g_battle_action_current_ability.ability_id;
}
