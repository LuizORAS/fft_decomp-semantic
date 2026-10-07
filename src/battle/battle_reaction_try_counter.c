#include "fft/battle.h"
#include "psx/types.h"

/* Queue a reaction to being targeted (reaction_id): against an ability whose flags_4 carry mask, and
 * not a Math Skill, a successful Brave roll sets it on the target with the ability in
 * last_received_attack. */
void battle_reaction_try_counter(u16 reaction_id, u32 mask) {
    battle_action_data_t* action;

    if (g_current_ability.skillset == SKILLSET_ID_MATH_SKILL)
        return;
    if ((g_current_ability.range_data.flags_4 & mask) == 0)
        return;
    if (battle_reaction_fails_brave_roll(g_battle_action_target) != 0)
        return;
    action = g_battle_action_target_data;
    action->reaction_id = reaction_id;
    action->last_received_attack = g_current_ability.ability_id;
}
