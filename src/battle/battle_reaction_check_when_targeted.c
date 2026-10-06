#include "fft/battle.h"
#include "psx/types.h"

/* Give the target its reactions to being targeted: the first it has of Sunken State, Caution, Dragon
 * Spirit, Brave Up, Faith Up, Counter Tackle, Counter Flood, Absorb Used MP, Counter and Counter
 * Magic. Counter Magic sits in the second reaction byte but is tested last, so Absorb Used MP or
 * Counter wins over it. battle_action_apply_target_result runs this before it tests the hit, so a
 * strike that missed provokes them too. Formula 7 and a unit that cannot react skip them. */
void battle_reaction_check_when_targeted(void) {
    battle_stats_t* unit;

    if (g_current_ability.formula == 7)
        return;
    if (battle_reaction_is_blocked(g_battle_action_target) != 0)
        return;
    unit = g_battle_action_target;
    if (unit->reaction_abilities[0] & BATTLE_REACTION_SET_1_SUNKEN_STATE) {
        battle_reaction_try_counter(ABILITY_ID_REACTION_SUNKEN_STATE, ABILITY_SECONDARY_FLAG_4_BLADE_GRASP_ELIGIBLE);
    } else if (unit->reaction_abilities[0] & BATTLE_REACTION_SET_1_CAUTION) {
        battle_reaction_try_counter(ABILITY_ID_REACTION_CAUTION, ABILITY_SECONDARY_FLAG_4_BLADE_GRASP_ELIGIBLE);
    } else if (unit->reaction_abilities[0] & BATTLE_REACTION_SET_1_DRAGON_SPIRIT) {
        battle_reaction_try_counter(ABILITY_ID_REACTION_DRAGON_SPIRIT, ABILITY_SECONDARY_FLAG_4_BLADE_GRASP_ELIGIBLE);
    } else if (unit->reaction_abilities[0] & BATTLE_REACTION_SET_1_BRAVE_UP) {
        battle_reaction_try_counter(ABILITY_ID_REACTION_BRAVE_UP, ABILITY_SECONDARY_FLAG_4_BLADE_GRASP_ELIGIBLE);
    } else if (unit->reaction_abilities[1] & BATTLE_REACTION_SET_2_FAITH_UP) {
        battle_reaction_try_on_mp_cost(ABILITY_ID_REACTION_FAITH_UP);
    } else if (unit->reaction_abilities[1] & BATTLE_REACTION_SET_2_COUNTER_TACKLE) {
        battle_reaction_try_counter(ABILITY_ID_REACTION_COUNTER_TACKLE, ABILITY_SECONDARY_FLAG_4_BLADE_GRASP_ELIGIBLE);
    } else if (unit->reaction_abilities[1] & BATTLE_REACTION_SET_2_COUNTER_FLOOD) {
        battle_reaction_try_counter(ABILITY_ID_REACTION_COUNTER_FLOOD, ABILITY_SECONDARY_FLAG_4_COUNTER_FLOOD);
    } else if (unit->reaction_abilities[2] & BATTLE_REACTION_SET_3_ABSORB_USED_MP) {
        battle_reaction_try_on_mp_cost(ABILITY_ID_REACTION_ABSORB_USED_MP);
    } else if (unit->reaction_abilities[2] & BATTLE_REACTION_SET_3_COUNTER) {
        battle_reaction_try_counter(ABILITY_ID_REACTION_COUNTER, ABILITY_SECONDARY_FLAG_4_BLADE_GRASP_ELIGIBLE);
    } else if (unit->reaction_abilities[1] & BATTLE_REACTION_SET_2_COUNTER_MAGIC) {
        battle_reaction_try_counter(ABILITY_ID_REACTION_COUNTER_MAGIC, ABILITY_SECONDARY_FLAG_4_COUNTER_MAGIC);
    }
}
