#include "fft/battle_action.h"
#include "psx/types.h"

/* Dispatch the first active stat-save or resource-restoring reaction. A weapon
 * that heals never triggers them. */
void battle_reaction_check_after_result(void) {
    battle_stats_t* target;
    u8 reaction_flags;

    if (g_battle_action_current_ability.formula == ABILITY_FORMULA_WEAPON_HEAL
        || battle_reaction_is_blocked(g_battle_action_target) != 0) {
        return;
    }
    target = g_battle_action_target;
    reaction_flags = target->ability_flags.reaction[0];
    if (reaction_flags & BATTLE_REACTION_SET_1_PA_SAVE) {
        battle_reaction_try_on_hp_damage(ABILITY_ID_REACTION_PA_SAVE);
    } else if (reaction_flags & BATTLE_REACTION_SET_1_MA_SAVE) {
        battle_reaction_try_on_hp_damage(ABILITY_ID_REACTION_MA_SAVE);
    } else if (reaction_flags & BATTLE_REACTION_SET_1_SPEED_SAVE) {
        battle_reaction_try_on_hp_damage(ABILITY_ID_REACTION_SPEED_SAVE);
    } else if (reaction_flags & BATTLE_REACTION_SET_1_REGENERATOR) {
        battle_reaction_try_on_hp_damage(ABILITY_ID_REACTION_REGENERATOR);
    } else {
        reaction_flags = target->ability_flags.reaction[1];
        if (reaction_flags & BATTLE_REACTION_SET_2_HP_RESTORE) {
            battle_reaction_try_while_critical(ABILITY_ID_REACTION_HP_RESTORE);
        } else if (reaction_flags & BATTLE_REACTION_SET_2_MP_RESTORE) {
            battle_reaction_try_while_critical(ABILITY_ID_REACTION_MP_RESTORE);
        } else if (reaction_flags & BATTLE_REACTION_SET_2_CRITICAL_QUICK) {
            battle_reaction_try_while_critical(ABILITY_ID_REACTION_CRITICAL_QUICK);
        } else if (reaction_flags & BATTLE_REACTION_SET_2_MEATBONE_SLASH) {
            battle_reaction_try_while_critical(ABILITY_ID_REACTION_MEATBONE_SLASH);
        } else {
            reaction_flags = target->ability_flags.reaction[2];
            if (reaction_flags & BATTLE_REACTION_SET_3_GILGAME_HEART) {
                battle_reaction_try_on_hp_damage(ABILITY_ID_REACTION_GILGAME_HEART);
            } else if (reaction_flags & BATTLE_REACTION_SET_3_AUTO_POTION) {
                battle_reaction_try_on_hp_damage(ABILITY_ID_REACTION_AUTO_POTION);
            }
        }
    }
}
