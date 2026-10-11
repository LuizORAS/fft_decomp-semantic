#include "fft/battle.h"
#include "psx/libetc.h"

/*
 * Simulate the current action and intervening events, then score unit state.
 *
 * Return -1 to suspend, 0 for a rejected or flag-update-only pass, and 1
 * after scoring. Save/restore helpers preserve selected unit data, not the
 * whole AI workspace; resumed phases skip fresh action preparation.
 */
s32 battle_ai_simulate_action_and_score(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_stats_t* unit;
    battle_ai_unit_decision_t* decision;
    battle_action_command_t* action;
    battle_ai_data_t* clear_cursor;
    battle_ai_data_t* decision_base;
    s32 unit_index;
    s32 status;
    s32 equipment_slot;
    s32 weight_offset;
    s32 weight;
    s32 score;
    u32 below_cost;

    if (g_battle_ai_data_base.resume_pending && g_battle_ai_data_base.action_simulation_phase) {
        battle_ai_restore_unit_state(1);
        /* Resume: re-enter the suspended turn-advance or scoring phase. */
        if (g_battle_ai_data_base.action_simulation_phase == BATTLE_AI_SIMULATION_ADVANCE_TURNS)
            goto advance_turns;
        if (g_battle_ai_data_base.action_simulation_phase == BATTLE_AI_SIMULATION_SCORE_STATE)
            goto score_state;
    }
    if (VSync(1) >= 441) {
        ai->action_simulation_phase = BATTLE_AI_SIMULATION_PREPARE_ACTION;
        return -1;
    }
    battle_ai_save_unit_state(0);
    ai->ability_processing_done = 0;
    /* Direct array iteration changes the bytes; retain a workspace-relative cursor. */
    for (unit_index = BATTLE_UNIT_SLOT_COUNT - 1,
        clear_cursor = (battle_ai_data_t*)((u8*)ai + (BATTLE_UNIT_SLOT_COUNT - 1));
        unit_index >= 0; unit_index--, clear_cursor = (battle_ai_data_t*)((u8*)clear_cursor - 1)) {
        clear_cursor->unit_acts_before_me[0] = 0;
    }
    ai->current_action.rank.priority = 0;
    ai->current_action.rank.base_hit_percent = 0xff;
    if (ai->current_action.act_before_move) {
        if (ai->current_action.command.skillset_id != 0
            && ai->current_action.command.skillset_id != SKILLSET_ID_DEFEND) {
            action = &ai->current_action.command;
            if (battle_action_call_attack_preparation_at_preview(action) == BATTLE_ATTACK_PREPARATION_IMMEDIATE
                && battle_ai_call_ability_processing(action) == 0) {
                battle_ai_restore_unit_state(0);
                ai->resume_pending = 0;
                return 0;
            }
            ai->acting_unit->action_taken = 1;
            battle_ai_simulate_movement_and_pickup();
        }
    } else {
        battle_ai_simulate_movement_and_pickup();
        if (ai->current_action.command.skillset_id != 0
            && ai->current_action.command.skillset_id != SKILLSET_ID_DEFEND) {
            action = &ai->current_action.command;
            if (battle_action_call_attack_preparation_at_preview(action) == BATTLE_ATTACK_PREPARATION_IMMEDIATE
                && battle_ai_call_ability_processing(action) == 0) {
                battle_ai_restore_unit_state(0);
                ai->resume_pending = 0;
                return 0;
            }
            ai->acting_unit->action_taken = 1;
        }
    }
    if (ai->current_action.command.skillset_id == SKILLSET_ID_DEFEND) {
        battle_status_inflict_defending_to_battle_id(ai->acting_unit_id);
        ai->current_action.rank.base_hit_percent = 100;
        ai->acting_unit->action_taken = 1;
    }
    if (ai->current_action.command.skillset_id == 0)
        ai->current_action.rank.base_hit_percent = 100;
    battle_action_end_turn(ai->acting_unit_id);
    ai->saved_between_turn_state = g_battle_between_turn_state;
    ai->saved_between_turn_resume_state = g_battle_between_turn_resume_state;
    ai->saved_turn_unit_id = g_battle_turn_unit_id;
    ai->simulated_turn_events = 0;
    for (;;) {
    advance_turns:
        if (VSync(1) >= 313) {
            battle_ai_save_unit_state(1);
            ai->action_simulation_phase = BATTLE_AI_SIMULATION_ADVANCE_TURNS;
            return -1;
        }
        ai->simulated_turn_events++;
        if (ai->simulated_turn_events >= 256)
            break;
        unit_index = battle_action_run_between_turn_control(BATTLE_BETWEEN_TURN_MODE_RUN);
        status = unit_index & 0xff00;
        unit_index &= 0xff;
        unit = &g_battle_unit_stats[unit_index];
        if (status == BATTLE_TURN_EVENT_NONE)
            break;
        if (status == BATTLE_TURN_EVENT_ACTION_RESULT) {
            battle_action_finalize_attack_and_flag_reactions(unit_index);
        } else if (status == BATTLE_TURN_EVENT_ABILITY_READY) {
            if (battle_ai_call_ability_processing(&unit->action_command) == 0) {
                g_battle_between_turn_state = ai->saved_between_turn_state;
                g_battle_between_turn_resume_state = ai->saved_between_turn_resume_state;
                g_battle_turn_unit_id = ai->saved_turn_unit_id;
                /* An early return duplicates this shared cleanup tail. */
                goto finish_without_scoring;
            }
            unit->charged_ability_ct = 0xff;
            unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARGING)]
                &= ~(BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARGING)
                    | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PERFORMING));
            unit->inflicted_status[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARGING)]
                &= ~(BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARGING)
                    | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PERFORMING));
        } else if (status == BATTLE_TURN_EVENT_UNIT_READY) {
            if (battle_ai_decide_status_ct_based(0, unit_index))
                ai->unit_acts_before_me[unit_index] = 1;
            unit->has_turn = 0;
        }
        for (unit_index = 0; unit_index < BATTLE_UNIT_SLOT_COUNT; unit_index++) {
            unit = &g_battle_unit_stats[unit_index];
            if (unit->entd_slot != BATTLE_ENTD_SLOT_NONE
                && (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARGING)]
                    & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARGING)
                        | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PERFORMING))))
                break;
        }
        if (unit_index == BATTLE_UNIT_ID_NONE)
            break;
    }
    g_battle_between_turn_state = ai->saved_between_turn_state;
    g_battle_between_turn_resume_state = ai->saved_between_turn_resume_state;
    g_battle_turn_unit_id = ai->saved_turn_unit_id;
score_state:
    if (VSync(1) >= 441) {
        battle_ai_save_unit_state(1);
        ai->action_simulation_phase = BATTLE_AI_SIMULATION_SCORE_STATE;
        return -1;
    }
    if (ai->targetability.live.target_setting_flags & BATTLE_AI_TARGET_SETTING_REFRESH_UNIT_STATUS_FLAGS) {
        for (unit_index = 0; unit_index < BATTLE_UNIT_SLOT_COUNT; unit_index++) {
            decision = &ai->unit_decisions[unit_index];
            unit = &g_battle_unit_stats[unit_index];
            decision->flags &= ~(BATTLE_AI_DECISION_TRANSIENT_DEAD | BATTLE_AI_DECISION_TRANSIENT_PETRIFIED
                | BATTLE_AI_DECISION_TRANSIENT_JUMP);
            if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD))
                && !(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_RERAISE)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_RERAISE)))
                decision->flags |= BATTLE_AI_DECISION_TRANSIENT_DEAD;
            if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_PETRIFY)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PETRIFY))
                decision->flags |= BATTLE_AI_DECISION_TRANSIENT_PETRIFIED;
            if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_JUMP)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_JUMP))
                decision->flags |= BATTLE_AI_DECISION_TRANSIENT_JUMP;
            if (!(decision->flags & BATTLE_AI_DECISION_CONSIDERED)
                && !(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_PETRIFY)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PETRIFY))
                && (!(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
                        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD))
                    || (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_RERAISE)]
                        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_RERAISE)))
                && !(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEATH_SENTENCE)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEATH_SENTENCE)))
                decision->flags |= BATTLE_AI_DECISION_CONSIDERED;
        }
    } else {
        battle_ai_clear_current_action_rank_byte();
        for (unit_index = 0; unit_index < BATTLE_UNIT_SLOT_COUNT; unit_index++) {
            status = ai->unit_compact_indices[unit_index];
            if (status == 0xff)
                continue;
            unit = &g_battle_unit_stats[unit_index];
            score = unit->hp;
            score <<= 7;
            score /= unit->max_hp;
            status = 0;
            /* A pointer to unit_decisions adds an addiu and changes scheduling;
             * retain the workspace-relative stride. */
            decision_base = (battle_ai_data_t*)((u8*)ai + unit_index * sizeof(battle_ai_unit_decision_t));
            for (weight_offset = 0; status < BATTLE_STATUS_COUNT; status++, weight_offset += 2) {
                if (!((unit->status_sets.current[status / 8] << (status % 8)) & 0x80))
                    continue;
                /* Indexing the s16 table changes register allocation; retain the byte cursor. */
                weight = *(s16*)((u8*)g_battle_ai_status_priority_weights + weight_offset);
                if (((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CONFUSION)]
                         & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CONFUSION)
                             | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_BLOOD_SUCK)))
                        || (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARM)]
                            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARM)))
                    && weight >= 0)
                    continue;
                switch (status) {
                case BATTLE_STATUS_ID_DARKNESS:
                    weight = weight * decision_base->unit_decisions[0].evade_mod / 4;
                    break;
                case BATTLE_STATUS_ID_SILENCE:
                    weight = weight * decision_base->unit_decisions[0].silence_mod / 4;
                    break;
                case BATTLE_STATUS_ID_CONFUSION:
                case BATTLE_STATUS_ID_BLOOD_SUCK:
                case BATTLE_STATUS_ID_CHARM:
                    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_SLOW)]
                            & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SLOW)
                                | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_STOP)))
                        || (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_SLEEP)]
                            & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SLEEP)
                                | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DONT_MOVE)
                                | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DONT_ACT))))
                        weight /= 2;
                    break;
                case BATTLE_STATUS_ID_SLOW:
                case BATTLE_STATUS_ID_STOP:
                case BATTLE_STATUS_ID_SLEEP:
                case BATTLE_STATUS_ID_DONT_MOVE:
                case BATTLE_STATUS_ID_DONT_ACT:
                    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CONFUSION)]
                            & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CONFUSION)
                                | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_BLOOD_SUCK)))
                        || (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARM)]
                            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARM)))
                        continue;
                    break;
                }
                score += weight;
            }
            if (!(unit->unit_flags & UNIT_FLAG_MONSTER)) {
                for (equipment_slot = 0; equipment_slot < 7; equipment_slot++) {
                    if (unit->equipment[equipment_slot]
                        == g_battle_ai_unit_snapshots[ai->unit_compact_indices[unit_index]].equipment[equipment_slot])
                        score += 51;
                }
            }
            if (unit->max_mp) {
                weight_offset = unit->mp;
                below_cost = weight_offset;
                weight_offset <<= 6;
                below_cost = below_cost < ai->unit_decisions[unit_index].lowest_mp_cost;
                weight_offset /= unit->max_mp;
                if (below_cost)
                    weight_offset = (u32)weight_offset >> 1;
                weight_offset = weight_offset * ai->unit_decisions[unit_index].mp_ability_mod / 4;
                score += weight_offset;
            }
            {
                u32 golem;

                weight_offset = g_battle_team_golem[(unit->team_flags >> 4) & 3];
                golem = weight_offset;
                weight_offset <<= 7;
                score += weight_offset / ai->present_unit_average_max_hp;
                if (golem < ai->team_golem[(unit->team_flags >> 4) & 3])
                    score--;
            }
            if (ai->unit_decisions[unit_index].enemy_flag)
                score = -score;
            ai->current_action.rank.priority += score;
        }
        if (g_main_action_menu_types_by_skillset[ai->considered_ability.skillset] == ACTION_MENU_TYPE_WEAPON_INVENTORY
            && ai->considered_ability.ability_id != ABILITY_ID_THROW_SHURIKEN
            && ai->considered_ability.ability_id != ABILITY_ID_THROW_BALL)
            ai->current_action.rank.base_hit_percent >>= 1;
        battle_ai_restore_unit_state(0);
        ai->resume_pending = 0;
        return 1;
    }
finish_without_scoring:
    battle_ai_restore_unit_state(0);
    ai->resume_pending = 0;
    return 0;
}
