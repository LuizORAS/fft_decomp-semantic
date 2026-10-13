#include "fft/battle_action.h"
#include "psx/types.h"

/* Dispatch the first active stat-save or resource-restoring reaction. A weapon
 * that heals never triggers them. */
void action_check_stat_save_and_restore_reaction_usability(void) {
    unit_stats_t* target;
    u8 reaction_flags;

    if (g_action_current_ability.formula == ABILITY_FORMULA_WEAPON_HEAL
        || action_is_reaction_blocked(g_action_target) != 0) {
        return;
    }
    target = g_action_target;
    reaction_flags = target->ability_flags.reaction[0];
    if (reaction_flags & BATTLE_REACTION_SET_1_PA_SAVE) {
        action_check_pa_save_ma_save_speed_save_regenerator_auto_potion_gilgame_heart_usability(
            ABILITY_ID_REACTION_PA_SAVE);
    } else if (reaction_flags & BATTLE_REACTION_SET_1_MA_SAVE) {
        action_check_pa_save_ma_save_speed_save_regenerator_auto_potion_gilgame_heart_usability(
            ABILITY_ID_REACTION_MA_SAVE);
    } else if (reaction_flags & BATTLE_REACTION_SET_1_SPEED_SAVE) {
        action_check_pa_save_ma_save_speed_save_regenerator_auto_potion_gilgame_heart_usability(
            ABILITY_ID_REACTION_SPEED_SAVE);
    } else if (reaction_flags & BATTLE_REACTION_SET_1_REGENERATOR) {
        action_check_pa_save_ma_save_speed_save_regenerator_auto_potion_gilgame_heart_usability(
            ABILITY_ID_REACTION_REGENERATOR);
    } else {
        reaction_flags = target->ability_flags.reaction[1];
        if (reaction_flags & BATTLE_REACTION_SET_2_HP_RESTORE) {
            action_check_critical_quick_hp_restore_mp_restore_meatbone_slash_usability(ABILITY_ID_REACTION_HP_RESTORE);
        } else if (reaction_flags & BATTLE_REACTION_SET_2_MP_RESTORE) {
            action_check_critical_quick_hp_restore_mp_restore_meatbone_slash_usability(ABILITY_ID_REACTION_MP_RESTORE);
        } else if (reaction_flags & BATTLE_REACTION_SET_2_CRITICAL_QUICK) {
            action_check_critical_quick_hp_restore_mp_restore_meatbone_slash_usability(
                ABILITY_ID_REACTION_CRITICAL_QUICK);
        } else if (reaction_flags & BATTLE_REACTION_SET_2_MEATBONE_SLASH) {
            action_check_critical_quick_hp_restore_mp_restore_meatbone_slash_usability(
                ABILITY_ID_REACTION_MEATBONE_SLASH);
        } else {
            reaction_flags = target->ability_flags.reaction[2];
            if (reaction_flags & BATTLE_REACTION_SET_3_GILGAME_HEART) {
                action_check_pa_save_ma_save_speed_save_regenerator_auto_potion_gilgame_heart_usability(
                    ABILITY_ID_REACTION_GILGAME_HEART);
            } else if (reaction_flags & BATTLE_REACTION_SET_3_AUTO_POTION) {
                action_check_pa_save_ma_save_speed_save_regenerator_auto_potion_gilgame_heart_usability(
                    ABILITY_ID_REACTION_AUTO_POTION);
            }
        }
    }
}
