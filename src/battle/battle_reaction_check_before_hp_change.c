#include "fft/battle.h"
#include "psx/types.h"

/* Give the target its reactions that change a hit before HP and MP move: the first it has of MP
 * Switch, Distribute and Damage Split. Formula 7 and a unit that cannot react skip them. */
void battle_reaction_check_before_hp_change(void) {
    battle_stats_t* unit;
    u8 flags;

    if (g_current_ability.formula == BATTLE_FORMULA_WEAPON_HEAL)
        return;
    if (battle_reaction_is_blocked(g_battle_action_target) != 0)
        return;
    unit = g_battle_action_target;
    flags = unit->reaction_abilities[2];
    if (flags & BATTLE_REACTION_SET_3_MP_SWITCH) {
        battle_reaction_try_mp_switch();
        return;
    }
    if (flags & BATTLE_REACTION_SET_3_DISTRIBUTE) {
        battle_reaction_try_distribute();
        return;
    }
    if (unit->reaction_abilities[3] & BATTLE_REACTION_SET_4_DAMAGE_SPLIT) {
        battle_reaction_try_damage_split();
    }
}
