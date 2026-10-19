#include "fft/battle_ai.h"

/* Choose the acting unit's Auto-Battle mode and main target. A player-controlled unit keeps its own
 * Auto-Battle setting; any other takes its ENTD behaviour, and keeps its special behaviour while it is
 * not Invited and its focus unit is still in play (without a focus unit, while more than one enemy is
 * left). Otherwise the player's team (guests) heads for the nearest healer or crystal below 45/128 HP
 * and for the nearest eligible enemy above; another team's supporter switches to Save Fading Life
 * while team support is available, and the rest target the nearest eligible enemy
 * (battle_ai_find_nearest_target, battle_ai_focus_on_target). */
void battle_ai_init_target_consideration(void) {
    battle_ai_unit_decision_t* decision;
    battle_ai_data_t* ai;
    battle_stats_t* acting;
    s32 flags;
    u8 targeting_flags;
    battle_ai_nearest_target_mode_e mode;
    s32 target;

    ai = &g_battle_ai_data_base;
    decision = g_battle_ai_data_base.acting_unit_decision;
    if (g_battle_ai_data_base.acting_unit_team == 0) {
        acting = g_battle_ai_data_base.acting_unit;
        if (acting->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) {
            ai->autobattle_setting = acting->auto_battle_setting;
            return;
        }
    }

    ai->autobattle_setting = ai->unit_autobattle_modes[ai->acting_unit_id];
    flags = decision->flags;
    if (flags & BATTLE_AI_DECISION_SPECIAL_BEHAVIOR) {
        if (!(ai->acting_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_INVITE)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_INVITE))) {
            if (decision->targeting_flags_2 & BATTLE_AI_TARGET_RUN_LIKE_A_RABBIT)
                return;
            if (ai->autobattle_setting)
                return;
            if (flags & BATTLE_AI_DECISION_FOCUS_TARGET) {
                battle_ai_transfer_unit_coordinates(decision->main_target_id, &decision->target);
                target = battle_ai_is_unit_out_of_play(decision->main_target_id);
            } else {
                target = ai->single_enemy_remaining;
            }
            if (target == 0)
                return;
        }
        decision->flags &= ~BATTLE_AI_DECISION_SPECIAL_BEHAVIOR;
    }

    if (ai->acting_unit_team == 0) {
        ai->autobattle_setting = BATTLE_AUTO_BATTLE_MANUAL;
        if ((ai->acting_unit->hp << 7) / ai->acting_unit->max_hp < 45) {
            mode = BATTLE_AI_NEAREST_HEALER_OR_CRYSTAL;
        } else {
            for (target = 0; target < BATTLE_UNIT_SLOT_COUNT; target++)
                ai->targetability.live.unit_targetable[target] = 1;
            mode = BATTLE_AI_NEAREST_ELIGIBLE_ENEMY;
        }
        target = battle_ai_find_nearest_target(mode);
        battle_ai_focus_on_target(target);
    } else {
        targeting_flags = decision->targeting_flags_1;
        if (!(targeting_flags & BATTLE_AI_TARGETING_FLAG_1_MOSTLY_SUPPORT)
            || (decision->targeting_flags_2 & BATTLE_AI_TARGET_TEAM_SUPPORT_UNAVAILABLE)) {
            ai->autobattle_setting = BATTLE_AUTO_BATTLE_MANUAL;
            for (target = 0; target < BATTLE_UNIT_SLOT_COUNT; target++)
                ai->targetability.live.unit_targetable[target] = 1;
            mode = BATTLE_AI_NEAREST_ELIGIBLE_ENEMY;
            target = battle_ai_find_nearest_target(mode);
            battle_ai_focus_on_target(target);
        } else {
            ai->autobattle_setting = BATTLE_AUTO_BATTLE_SAVE_FADING_LIFE;
        }
    }
}
