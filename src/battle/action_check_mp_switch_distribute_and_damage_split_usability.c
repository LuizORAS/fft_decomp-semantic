#include "fft/battle_action.h"
#include "psx/types.h"

/* Dispatch the target's first active MP Switch, Distribute, or Damage Split reaction. */
void action_check_mp_switch_distribute_and_damage_split_usability(void) {
    unit_stats_t* unit;
    u8 flags;

    if (g_action_current_ability.formula == ABILITY_FORMULA_WEAPON_HEAL)
        return;
    if (action_is_reaction_blocked(g_action_target) != 0)
        return;
    unit = g_action_target;
    flags = unit->ability_flags.reaction[2];
    if (flags & BATTLE_REACTION_SET_3_MP_SWITCH) {
        action_check_mp_switch_usability();
        return;
    }
    if (flags & BATTLE_REACTION_SET_3_DISTRIBUTE) {
        action_check_distribute_usability();
        return;
    }
    if (unit->ability_flags.reaction[3] & BATTLE_REACTION_SET_4_DAMAGE_SPLIT) {
        action_check_damage_split_usability();
    }
}
