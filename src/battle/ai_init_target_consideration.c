#include "fft/battle_ai.h"

/*
 * Prepare the acting unit's automatic targeting policy and focused target.
 *
 * Player-team units under direct control retain their configured automatic
 * behavior. Other units may abandon special focus behavior when its target is
 * no longer eligible, then select an appropriate nearby unit.
 */
void ai_init_target_consideration(void) {
    ai_unit_decision_t* decision;
    ai_data_t* ai;
    battle_stats_t* acting;
    s32 flags;
    u8 targeting_flags;
    ai_nearest_target_mode_e mode;
    s32 target;

    ai = &g_ai_data_base;
    decision = g_ai_data_base.acting_unit_decision;
    if (g_ai_data_base.acting_unit_team == 0) {
        acting = g_ai_data_base.acting_unit;
        if (acting->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) {
            ai->autobattle_setting = acting->auto_battle_setting;
            return;
        }
    }

    ai->autobattle_setting = ai->unit_autobattle_modes[ai->acting_unit_id];
    flags = decision->flags;
    if (flags & AI_DECISION_SPECIAL_BEHAVIOR) {
        if (!(ai->acting_unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_INVITE)]
                & STATUS_BYTE_MASK(STATUS_ID_INVITE))) {
            if (decision->targeting_flags_2 & AI_TARGET_CONSERVE_CT)
                return;
            if (ai->autobattle_setting)
                return;
            if (flags & AI_DECISION_FOCUS_TARGET) {
                ai_transfer_unit_coordinates(decision->main_target_id, &decision->target);
                target = ai_check_target_type(decision->main_target_id);
            } else {
                target = ai->single_enemy_remaining;
            }
            if (target == 0)
                return;
        }
        decision->flags &= ~AI_DECISION_SPECIAL_BEHAVIOR;
    }

    if (ai->acting_unit_team == 0) {
        ai->autobattle_setting = AI_AUTOBATTLE_MANUAL;
        if ((ai->acting_unit->hp << 7) / ai->acting_unit->max_hp < 45) {
            mode = AI_NEAREST_HEALER_OR_CRYSTAL;
        } else {
            for (target = 0; target < BATTLE_UNIT_SLOT_COUNT; target++)
                ai->targetability.live.unit_targetable[target] = 1;
            mode = AI_NEAREST_ELIGIBLE_ENEMY;
        }
        target = ai_find_nearest_target(mode);
        ai_focus_on_target(target);
    } else {
        targeting_flags = decision->targeting_flags_1;
        if (!(targeting_flags & AI_TARGETING_FLAG_1_MOSTLY_SUPPORT)
            || (decision->targeting_flags_2 & AI_TARGET_TEAM_SUPPORT_UNAVAILABLE)) {
            ai->autobattle_setting = AI_AUTOBATTLE_MANUAL;
            for (target = 0; target < BATTLE_UNIT_SLOT_COUNT; target++)
                ai->targetability.live.unit_targetable[target] = 1;
            mode = AI_NEAREST_ELIGIBLE_ENEMY;
            target = ai_find_nearest_target(mode);
            ai_focus_on_target(target);
        } else {
            ai->autobattle_setting = AI_AUTOBATTLE_FADING_LIFE;
        }
    }
}
