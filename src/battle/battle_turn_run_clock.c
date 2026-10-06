#include "fft/battle.h"
#include "psx/types.h"

/* Run the turn clock (g_battle_turn_clock_state) until a phase yields a turn event, returned
 * as unit id | event. A tick adds each unit's Speed to its CT (half under Slow, 1.5 times
 * under Haste, at most 254); the unit with the highest CT above 99 gets its turn (UNIT_READY)
 * after its status upkeep, which can yield an ACTION_RESULT instead, while a jumping unit waits
 * at CT 99. With no turn due, charged abilities count down and resolve (ABILITY_READY), each
 * followed by an immediate turn for a unit at CT 255 (Quick) and by Mime copies; then timed
 * statuses expire and their results come out (ACTION_RESULT). After a turn ends, the turn
 * unit's poison and regen, terrain poison and Transparent removal come out the same way, then
 * the Mime copies. Mode 1 resets the clock; mode 2, which nothing calls, returns the first unit
 * able to act. Returns BATTLE_TURN_EVENT_NONE after 1200 phases without an event.
 *
 * Phases 2, 4, 5 and 13 store the next state on their loop's closing line.
 * Under -gcoff a separate line puts a line note between the loop end and the
 * state constant, and loop.c then keeps 3, 5, 6 and 14 inside the state loop
 * (cross-jumped `li v0,N` tails). With no note between them the constant set
 * directly follows the loop and is hoisted with the table base, as in the
 * target (a spilled invariant rematerialised as `li a2,N`). */
s32 battle_turn_run_clock(s32 mode) {
    battle_stats_t* unit;
    s32 result;
    s32 count;
    s32 i;
    s32 best;
    s32 ret;
    s32 flags;
    u16 max_ct;
    u16 speed;
    u16 ct;
    u8 value;

    result = 0;
    count = 0;
    if (mode == 1) {
        g_battle_turn_clock_state = 0;
        g_battle_turn_unit_id = -1;
        for (i = 3; i >= 0; i--) {
            g_battle_team_golem[i] = 0;
        }
        g_current_ability_attacker.mimic_pending = 0;
        for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
            unit = &g_battle_unit_stats[i];
            unit->ct = 0;
            unit->has_turn = 0;
            unit->ability_outcome = 0;
            unit->charged_ability_ct = 0xff;
        }
        battle_ai_restore_considered_action_data();
        return 0xe000;
    }
    if (mode == 2) {
        for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
            unit = &g_battle_unit_stats[i];
            if (unit->entd_slot != BATTLE_ENTD_SLOT_NONE) {
                if (!(unit->status_sets.current[0] & 0x64)) {
                    if (!(unit->status_sets.current[1] & 0x81)) {
                        return i | 0xd000;
                    }
                }
            }
        }
        return 0xdf00;
    }
    while (result == 0) {
        switch (g_battle_turn_clock_state) {
        case 0:
            for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
                unit = &g_battle_unit_stats[i];
                flags = battle_turn_get_status_flags(unit);
                if (!(flags & BATTLE_TURN_STATUS_CT_FROZEN)) {
                    speed = unit->attributes[UNIT_ATTRIBUTE_SPEED];
                    if (flags & BATTLE_TURN_STATUS_SLOW) {
                        speed >>= 1;
                    } else if (flags & BATTLE_TURN_STATUS_HASTE) {
                        speed += speed >> 1;
                    }
                    ct = unit->ct + speed;
                    if (ct >= 0xff) {
                        ct = 0xfe;
                    }
                    unit->ct = ct;
                }
            }
            g_battle_turn_clock_state = 1;
            break;
        case 1:
            max_ct = 99;
            best = 0xff;
            for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
                unit = &g_battle_unit_stats[i];
                flags = battle_turn_get_status_flags(unit);
                if (!(flags & BATTLE_TURN_STATUS_CT_FROZEN)) {
                    if (max_ct < unit->ct) {
                        best = i;
                        max_ct = unit->ct;
                    }
                }
            }
            if (best != 0xff) {
                unit = &g_battle_unit_stats[best];
                if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_JUMP)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_JUMP)) {
                    unit->ct = 99;
                    continue;
                }
                if (unit->ct == 0xff) {
                    unit->ct = 0;
                } else {
                    unit->ct = max_ct % 100;
                }
                ret = battle_status_build_upkeep_action(best, unit);
                if (ret != 0) {
                    result = best | BATTLE_TURN_EVENT_ACTION_RESULT;
                    if (ret & 1) {
                        if (ret == 1) {
                            main_unit_increase_casualty_counters(unit);
                        }
                        return result;
                    }
                }
                flags = battle_turn_get_status_flags(unit);
                if (!(flags
                        & (BATTLE_TURN_STATUS_CT_FROZEN | BATTLE_TURN_STATUS_INCAPACITATED | BATTLE_TURN_STATUS_DEAD))
                    || ret == 2) {
                    unit->has_turn = 1;
                    unit->movement_taken = 0;
                    unit->action_taken = 0;
                    unit->transparent_removal_flag = 1;
                    if (ret == 0) {
                        result = best | BATTLE_TURN_EVENT_UNIT_READY;
                    }
                    g_battle_turn_clock_resume_state = g_battle_turn_clock_state;
                    g_battle_turn_unit_id = best;
                    g_battle_turn_clock_state = 10;
                    return result;
                }
                continue;
            }
            g_battle_turn_clock_state = 2;
            break;
        case 2:
            i = 0;
            do {
                unit = &g_battle_unit_stats[i];
                flags = battle_turn_get_status_flags(unit);
                if (!(flags
                        & (BATTLE_TURN_STATUS_CT_FROZEN | BATTLE_TURN_STATUS_INCAPACITATED | BATTLE_TURN_STATUS_DEAD
                            | BATTLE_TURN_STATUS_SLEEP))) {
                    value = unit->charged_ability_ct;
                    if (value != 0xff && value != 0) {
                        value--;
                        unit->charged_ability_ct = value;
                    }
                }
                /* clang-format off */
            } while (++i < BATTLE_UNIT_SLOT_COUNT); g_battle_turn_clock_state = 3;
            /* clang-format on */
            break;
        case 3:
            for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
                unit = &g_battle_unit_stats[i];
                flags = battle_turn_get_status_flags(unit);
                if (!(flags
                        & (BATTLE_TURN_STATUS_CT_FROZEN | BATTLE_TURN_STATUS_INCAPACITATED | BATTLE_TURN_STATUS_DEAD
                            | BATTLE_TURN_STATUS_SLEEP))
                    && unit->charged_ability_ct == 0) {
                    if (unit->status_sets.current[0] & 1) {
                        main_unit_copy_last_ability_ct(unit);
                        unit->charged_ability_ct = unit->ability_ct;
                    } else {
                        unit->charged_ability_ct = 0xff;
                    }
                    g_battle_turn_clock_state = 9;
                    result = i | BATTLE_TURN_EVENT_ABILITY_READY;
                    return result;
                }
            }
            g_battle_turn_clock_state = 4;
            break;
        case 4:
            i = 0;
            do {
                unit = &g_battle_unit_stats[i];
                if (unit->entd_slot != BATTLE_ENTD_SLOT_NONE
                    && !(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_JUMP)]
                        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_JUMP))) {
                    battle_status_update_expiring(i);
                }
                /* clang-format off */
            } while (++i < BATTLE_UNIT_SLOT_COUNT); g_battle_turn_clock_state = 5;
            /* clang-format on */
            break;
        case 5:
            i = 0;
            do {
                unit = &g_battle_unit_stats[i];
                g_battle_turn_pending_flags[i] = 0;
                if (unit->entd_slot != BATTLE_ENTD_SLOT_NONE
                    && !(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_JUMP)]
                        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_JUMP))) {
                    g_battle_turn_pending_flags[i] = unit->action.attack_type;
                }
                /* clang-format off */
            } while (++i < BATTLE_UNIT_SLOT_COUNT); g_battle_turn_clock_state = 6;
            /* clang-format on */
            break;
        case 6:
            for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
                if (g_battle_turn_pending_flags[i] != 0) {
                    g_battle_turn_pending_flags[i] = 0;
                    result = i | BATTLE_TURN_EVENT_ACTION_RESULT;
                    return result;
                }
            }
            g_battle_turn_clock_state = 0;
            break;
        case 9:
            max_ct = 0xfe;
            best = 0xff;
            for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
                flags = battle_turn_get_status_flags(&g_battle_unit_stats[i]);
                if (!(flags & BATTLE_TURN_STATUS_CT_FROZEN)) {
                    if (max_ct < g_battle_unit_stats[i].ct) {
                        best = i;
                        max_ct = g_battle_unit_stats[i].ct;
                    }
                }
            }
            if (best != 0xff) {
                unit = &g_battle_unit_stats[best];
                unit->has_turn = 1;
                unit->transparent_removal_flag = 1;
                unit->ct = 0;
                unit->movement_taken = 0;
                unit->action_taken = 0;
                g_battle_turn_clock_resume_state = 9;
                g_battle_turn_unit_id = best;
                g_battle_turn_clock_state = 10;
                result = best | BATTLE_TURN_EVENT_UNIT_READY;
                return result;
            }
            g_battle_turn_clock_resume_state = 3;
            g_battle_turn_clock_state = 13;
            break;
        case 10:
            for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
                unit = &g_battle_unit_stats[i];
                if (unit->has_turn != 0) {
                    s32 unit_status;
                    s32 end_turn;

                    unit_status = battle_turn_get_status_flags(unit);
                    end_turn = battle_turn_should_end(unit);
                    if ((unit_status & BATTLE_TURN_STATUS_CT_FROZEN) || end_turn != 0) {
                        battle_turn_end(i);
                        unit->has_turn = 0;
                    } else {
                        result = i | BATTLE_TURN_EVENT_UNIT_READY;
                        return result;
                    }
                }
            }
            g_battle_turn_clock_state = 19;
            break;
        case 19:
            g_battle_turn_clock_state = 15;
            if (g_battle_turn_unit_id != -1) {
                unit = &g_battle_unit_stats[g_battle_turn_unit_id];
                if (battle_status_apply_poison_and_regen(unit)) {
                    result = g_battle_turn_unit_id | BATTLE_TURN_EVENT_ACTION_RESULT;
                    return result;
                }
            }
            break;
        case 15:
            g_battle_turn_clock_state = 17;
            if (g_battle_turn_unit_id != -1) {
                unit = &g_battle_unit_stats[g_battle_turn_unit_id];
                if (battle_prepare_terrain_poison(unit)) {
                    result = g_battle_turn_unit_id | BATTLE_TURN_EVENT_ACTION_RESULT;
                    return result;
                }
            }
            break;
        case 17:
            g_battle_turn_clock_state = 13;
            if (g_battle_turn_unit_id != -1) {
                unit = &g_battle_unit_stats[g_battle_turn_unit_id];
                if (battle_status_remove_transparent(unit)) {
                    result = g_battle_turn_unit_id | BATTLE_TURN_EVENT_ACTION_RESULT;
                    return result;
                }
            }
            break;
        case 13:
            if (g_current_ability_attacker.mimic_pending != 0) {
                i = 0;
                do {
                    unit = &g_battle_unit_stats[i];
                    g_battle_turn_pending_flags[i] = 0;
                    if (unit->job_id == JOB_ID_MIME && unit->entd_slot != BATTLE_ENTD_SLOT_NONE) {
                        g_battle_turn_pending_flags[i] = 1;
                    }
                    /* clang-format off */
                } while (++i < BATTLE_UNIT_SLOT_COUNT); g_battle_turn_clock_state = 14;
                /* clang-format on */
            } else {
                g_battle_turn_clock_state = g_battle_turn_clock_resume_state;
            }
            break;
        case 14:
            g_current_ability_attacker.mimic_pending = 0;
            for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
                unit = &g_battle_unit_stats[i];
                if (g_battle_turn_pending_flags[i] != 0 && unit->entd_slot != BATTLE_ENTD_SLOT_NONE) {
                    g_battle_turn_pending_flags[i] = 0;
                    ret = battle_action_prepare_mimic(unit);
                    if (ret == 1) {
                        result = i | BATTLE_TURN_EVENT_ABILITY_READY;
                        return result;
                    }
                    if (ret == -1) {
                        result = i | BATTLE_TURN_EVENT_MIME;
                        return result;
                    }
                }
            }
            g_battle_turn_clock_state = g_battle_turn_clock_resume_state;
            break;
        }
        count++;
        if (count >= 1201) {
            break;
        }
    }
    return BATTLE_TURN_EVENT_NONE;
}
