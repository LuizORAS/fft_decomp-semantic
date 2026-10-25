#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/* Fight for life: attack the Auto-Battle target. When the target is out of play or not an enemy,
 * the unit drops its ENTD special behavior and takes the rider of a ridden mount (unless both are
 * Transparent), else the nearest eligible enemy, else the first Transparent enemy present. It then
 * scores the tiles, moves around the target (battle_ai_build_targeted_movement), keeps a charged action
 * when that scores better, ranks its abilities on the target, then on every unit worth targeting, and
 * takes the first pass's ranked action; with none, it only moves (and Defends when Defend is its
 * support ability). Returns -1 to suspend, else 0. */
s32 battle_ai_select_fight_for_life_action(void) {
    s32 target = g_battle_ai_data_base.acting_unit->auto_battle_target;
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_stats_t* unit;
    s32 i, result;

    /* Reload the persistent target before every resume dispatch. */
    if (g_battle_ai_data_base.resume_pending) {
        switch (g_battle_ai_data_base.action_selection_phase) {
        case BATTLE_AI_TARGETED_SELECTION_SCORE_TILES:
            goto score_tiles;
        case BATTLE_AI_TARGETED_SELECTION_MOVEMENT:
            goto movement;
        case BATTLE_AI_TARGETED_SELECTION_CHARGING:
            goto charging;
        case BATTLE_AI_TARGETED_SELECTION_TARGET_ABILITIES:
            goto target_abilities;
        default:
            goto other_abilities;
        }
    }
    if (battle_ai_is_unit_out_of_play(target) || !ai->unit_decisions[target].enemy_flag) {
        g_battle_ai_data_base.acting_unit_decision->flags &= ~BATTLE_AI_DECISION_SPECIAL_BEHAVIOR;
        unit = &g_battle_unit_stats[target];
        if (unit->mount_info & BATTLE_MOUNT_INFO_FLAG_MOUNT) {
            if (!((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TRANSPARENT)]
                      & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TRANSPARENT))
                    && (g_battle_unit_stats[unit->mount_info & BATTLE_MOUNT_INFO_PARTNER_ID_MASK]
                            .status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TRANSPARENT)]
                        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TRANSPARENT))))
                target = unit->mount_info & BATTLE_MOUNT_INFO_PARTNER_ID_MASK;
        } else {
            /* A direct member pointer folds the target's base cursor and field
             * displacement together. Derive the member offset to retain its stores. */
            u32 cursor;
            s32 included = 1;
            i = BATTLE_UNIT_SLOT_COUNT - 1;
            cursor = (u32)ai + i;
            do {
                *(u8*)(cursor + ((u32)&ai->targetability.live.unit_targetable - (u32)ai)) = included;
                i--;
                cursor--;
            } while (i >= 0);
            target = battle_ai_find_nearest_target(BATTLE_AI_NEAREST_ELIGIBLE_ENEMY);
            if (target == ai->acting_unit_id) {
                for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
                    unit = &g_battle_unit_stats[i];
                    if (unit->entd_slot != BATTLE_ENTD_SLOT_NONE && ai->unit_decisions[i].enemy_flag
                        && (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TRANSPARENT)]
                            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TRANSPARENT))) {
                        target = i;
                        break;
                    }
                }
            }
        }
    }
    ai->acting_unit->auto_battle_target = target;
    battle_ai_focus_on_target(target);
    ai->movement_scenario = 0;
score_tiles:
    if (battle_ai_score_reachable_tiles() == -1) {
        ai->action_selection_phase = BATTLE_AI_TARGETED_SELECTION_SCORE_TILES;
        return -1;
    }
movement:
    if (battle_ai_build_targeted_movement() == -1) {
        ai->action_selection_phase = BATTLE_AI_TARGETED_SELECTION_MOVEMENT;
        return -1;
    }
    ai->movement_scenario = 2;
    battle_ai_set_movement_baseline();
    ai->movement_scenario = 0;
charging:
    result = battle_ai_evaluate_charged_action();
    switch (result) {
    case -1:
        ai->action_selection_phase = BATTLE_AI_TARGETED_SELECTION_CHARGING;
        return -1;
    case 1:
        return 0;
    }
    ai->movement_scenario = 0;
    if (battle_ai_select_active_units(BATTLE_AI_UNIT_FILTER_SPECIFIC, target)) {
    target_abilities:
        if (battle_ai_evaluate_ability_list() == -1) {
            ai->action_selection_phase = BATTLE_AI_TARGETED_SELECTION_TARGET_ABILITIES;
            return -1;
        }
        if (battle_ai_select_ranked_action())
            return 0;
    }
    ai->movement_scenario = 1;
    if (battle_ai_select_active_units(BATTLE_AI_UNIT_FILTER_ANY, 0)) {
    other_abilities:
        if (battle_ai_evaluate_ability_list() == -1) {
            ai->action_selection_phase = BATTLE_AI_TARGETED_SELECTION_OTHER_ABILITIES;
            return -1;
        }
        if (battle_ai_select_ranked_action())
            return 0;
    }
    ai->movement_scenario = 2;
    battle_ai_select_move_only_action();
    return 0;
}
