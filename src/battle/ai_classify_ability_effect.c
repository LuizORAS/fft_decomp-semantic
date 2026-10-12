#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/*
 * Return the AI's usefulness category for one target.
 *
 * This is the acting side's heuristic, not the sign of HP damage or a success
 * guarantee. Nonzero check_reflect enables the timing-aware Reflect restriction.
 */
ai_ability_effect_e ai_classify_ability_effect(s32 unit_id, s32 check_reflect) {
    ai_data_t* ai = &g_ai_data_base;
    battle_stats_t* unit = &g_battle_unit_stats[unit_id];
    ai_unit_decision_t* decision = &ai->unit_decisions[unit_id];
    s32 ct = g_ai_data_base.considered_ability.ct;
    s32 i;
    s32 bit;
    s32 mask;
    s32 status;
    s16 ability_id;

    if ((g_ai_data_base.considered_ability.ai_flags.word & AI_ABILITY_ENEMIES_ONLY) && !decision->enemy_flag)
        return AI_ABILITY_EFFECT_NONE;
    if ((ai->considered_ability.ai_flags.word & AI_ABILITY_ALLIES_ONLY) && decision->enemy_flag)
        return AI_ABILITY_EFFECT_NONE;
    if (unit_id == ai->acting_unit_id) {
        ability_id = ai->considered_ability.ability_id;
        if (ability_id < ABILITY_ID_ITEM_FIRST
            && (g_main_ability_range_data[ability_id].flags_2 & ABILITY_SECONDARY_FLAG_2_CANNOT_HIT_CASTER))
            return AI_ABILITY_EFFECT_NONE;
    }
    if (check_reflect && (ai->considered_ability.ai_flags.word & AI_ABILITY_REFLECTABLE)
        && ai_is_status_active_through_delay(ct, unit, BATTLE_STATUS_ID_REFLECT))
        return AI_ABILITY_EFFECT_NONE;
    if ((ai->considered_ability.ai_flags.word & AI_ABILITY_AFFECTED_BY_FAITH)
        && ai_is_status_active_through_delay(ct, unit, BATTLE_STATUS_ID_INNOCENT))
        return AI_ABILITY_EFFECT_NONE;
    if (ai_is_status_active_through_delay(ct, unit, BATTLE_STATUS_ID_JUMP))
        return AI_ABILITY_EFFECT_NONE;

    /* Death/revival timing determines which effects can still matter. */
    i = 0;
    if ((ai->considered_ability.ai_flags.word & AI_ABILITY_CANCEL_STATUS)
        && (ai->considered_ability.parameters.bytes.status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD)))
        i = 1;
    if (decision->targeting_flags_2 & AI_TARGET_DEAD_WITH_RERAISE) {
        s32 ability_ct = ai->considered_ability.ct;
        if (ability_ct < ai_calculate_clockticks_until_unit_acts(unit))
            return AI_ABILITY_EFFECT_NONE;
        if (i)
            return AI_ABILITY_EFFECT_NONE;
    }
    if (decision->targeting_flags_2 & AI_TARGET_DEAD_WITHOUT_RERAISE) {
        s32 ability_ct = ai->considered_ability.ct;
        if (ai_calculate_clockticks_until_death_counter_expires(unit) < ability_ct)
            return AI_ABILITY_EFFECT_NONE;
        if (!i)
            return AI_ABILITY_EFFECT_NONE;
    }
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_PETRIFY)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PETRIFY))
        && (!(ai->considered_ability.ai_flags.word & AI_ABILITY_CANCEL_STATUS)
            || !(ai->considered_ability.parameters.bytes
                     .status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_PETRIFY)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PETRIFY))))
        return AI_ABILITY_EFFECT_NONE;
    if (ai_is_status_active_through_delay(ct, unit, BATTLE_STATUS_ID_WALL)
        && (!(ai->considered_ability.ai_flags.word & AI_ABILITY_CANCEL_STATUS)
            || !(ai->considered_ability.parameters.bytes
                     .status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_WALL)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_WALL))))
        return AI_ABILITY_EFFECT_NONE;

    i = ai->considered_ability.element;
    if (i) {
        if (i == (i & unit->elemental_affinity[ELEMENTAL_AFFINITY_NULLIFY])
            || (i == 8
                && (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FLOAT)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FLOAT)))) {
            return AI_ABILITY_EFFECT_NONE;
        }
    }

    if (ai->considered_ability.ai_flags.word & AI_ABILITY_TARGET_ENEMIES) {
        if (ai->considered_ability.ai_flags.word & AI_ABILITY_UNDEAD_REVERSE) {
            if (decision->enemy_flag) {
                if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_UNDEAD)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_UNDEAD))
                    return AI_ABILITY_EFFECT_ADVERSE;
            } else if (!(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_UNDEAD)]
                           & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_UNDEAD))) {
                return AI_ABILITY_EFFECT_ADVERSE;
            }
        } else if (decision->enemy_flag) {
            if (unit->elemental_affinity[ELEMENTAL_AFFINITY_ABSORB] & ai->considered_ability.element)
                return AI_ABILITY_EFFECT_ADVERSE;
        } else {
            if (ai_is_status_active_through_delay(ct, unit, BATTLE_STATUS_ID_CHARM)
                && ai_check_unit_gets_turn_without_status(unit, BATTLE_STATUS_ID_CHARM))
                i = 1;
            else
                i = 0;
            if ((ai->considered_ability.ai_flags.word & AI_ABILITY_AFFECTS_HP)
                && g_main_action_menu_types_by_skillset[ai->considered_ability.skillset] != ACTION_MENU_TYPE_CHARGE) {
                if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CONFUSION)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CONFUSION))
                    return AI_ABILITY_EFFECT_USEFUL;
                if (ai_is_status_active_through_delay(ct, unit, BATTLE_STATUS_ID_SLEEP))
                    return AI_ABILITY_EFFECT_USEFUL;
                if (i)
                    return AI_ABILITY_EFFECT_USEFUL;
            }
            if ((unit->elemental_affinity[ELEMENTAL_AFFINITY_ABSORB] & ai->considered_ability.element)
                && (decision->targeting_flags_2 & AI_TARGET_HP_BELOW_HALF))
                return AI_ABILITY_EFFECT_USEFUL;
            if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CONFUSION)]
                    & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CONFUSION)
                        | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_BLOOD_SUCK)))
                || i) {
                if (ai->considered_ability.ai_flags.word & AI_ABILITY_ADD_STATUS) {
                    /* Early acceptance of a disabling effect bypasses the generic
                     * already-present/immunity filters below. Status bytes 1..4
                     * are loaded together as an aligned 32-bit value by the target. */
                    s32 status_mask = ai->considered_ability.parameters.words.status_tail
                        & (AI_STATUS_TAIL_PACKED_MASK(BATTLE_STATUS_ID_SLOW)
                            | AI_STATUS_TAIL_PACKED_MASK(BATTLE_STATUS_ID_STOP)
                            | AI_STATUS_TAIL_PACKED_MASK(BATTLE_STATUS_ID_SLEEP)
                            | AI_STATUS_TAIL_PACKED_MASK(BATTLE_STATUS_ID_DONT_MOVE)
                            | AI_STATUS_TAIL_PACKED_MASK(BATTLE_STATUS_ID_DONT_ACT));
                    if (status_mask)
                        return AI_ABILITY_EFFECT_USEFUL;
                }
            }
            return AI_ABILITY_EFFECT_ADVERSE;
        }
        if ((ai->considered_ability.ai_flags.word & AI_ABILITY_AFFECTS_HP)
            && !(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEATH_SENTENCE)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEATH_SENTENCE)))
            return AI_ABILITY_EFFECT_USEFUL;
        if ((ai->considered_ability.ai_flags.word & AI_ABILITY_AFFECTS_MP) && unit->mp > decision->lowest_mp_cost)
            return AI_ABILITY_EFFECT_USEFUL;
    }

    if (ai->considered_ability.ai_flags.word & AI_ABILITY_TARGET_ALLIES) {
        if (ai->considered_ability.ai_flags.word & AI_ABILITY_UNDEAD_REVERSE) {
            if (decision->enemy_flag) {
                if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_UNDEAD)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_UNDEAD))
                    return AI_ABILITY_EFFECT_USEFUL;
                return AI_ABILITY_EFFECT_ADVERSE;
            }
            if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_UNDEAD)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_UNDEAD))
                return AI_ABILITY_EFFECT_ADVERSE;
        } else if (decision->enemy_flag)
            return AI_ABILITY_EFFECT_ADVERSE;
        /* Death Sentence: the Add Reraise shortcut likewise bypasses the
         * generic already-present/immunity filters. */
        if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEATH_SENTENCE)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEATH_SENTENCE)) {
            if ((ai->considered_ability.ai_flags.word & AI_ABILITY_ADD_STATUS)
                && (ai->considered_ability.parameters.bytes
                        .status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_RERAISE)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_RERAISE)))
                return AI_ABILITY_EFFECT_USEFUL;
            return AI_ABILITY_EFFECT_NONE;
        }
        if ((ai->considered_ability.ai_flags.word & AI_ABILITY_AFFECTS_HP)
            && (decision->targeting_flags_2 & AI_TARGET_HP_BELOW_HALF))
            return AI_ABILITY_EFFECT_USEFUL;
        if ((ai->considered_ability.ai_flags.word & AI_ABILITY_AFFECTS_MP)
            && (decision->targeting_flags_2 & AI_TARGET_MP_CONSTRAINED))
            return AI_ABILITY_EFFECT_USEFUL;
    }

    if (!(ai->considered_ability.ai_flags.word & (AI_ABILITY_TARGET_ALLIES | AI_ABILITY_TARGET_ENEMIES))) {
        if (ai->considered_ability.ai_flags.word & AI_ABILITY_AFFECTS_HP)
            return AI_ABILITY_EFFECT_USEFUL;
        if (ai->considered_ability.ai_flags.word & AI_ABILITY_AFFECTS_MP)
            return AI_ABILITY_EFFECT_USEFUL;
    }
    if (ai->considered_ability.ai_flags.word & AI_ABILITY_ADD_STATUS) {
        for (i = 0; i < BATTLE_STATUS_BYTE_COUNT; i++) {
            mask = ai->considered_ability.parameters.bytes.status_infliction[i];
            mask ^= mask & unit->status_sets.current[i];
            if (mask) {
                mask ^= mask & unit->status_sets.immunity[i];
                if (mask) {
                    switch (i) {
                    case 1:
                        if ((mask & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SILENCE)) && !decision->silence_mod)
                            mask ^= BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SILENCE);
                        if ((mask & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DARKNESS)) && decision->evade_mod < 2)
                            mask ^= BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DARKNESS);
                        break;
                    case 2:
                        if ((mask & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_RERAISE))
                            && !(decision->targeting_flags_2 & AI_TARGET_HP_BELOW_HALF))
                            mask ^= BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_RERAISE);
                        if ((mask & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG)) && !decision->enemy_flag)
                            mask ^= BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG);
                        if (!(decision->targeting_flags_2 & AI_TARGET_HP_BELOW_THREE_QUARTERS))
                            mask &= (u8)~BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FLOAT);
                        break;
                    case 3:
                        if ((mask & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_REGEN))
                            && !(decision->targeting_flags_2 & AI_TARGET_HP_BELOW_HALF))
                            mask ^= BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_REGEN);
                        if (!(decision->targeting_flags_2 & AI_TARGET_HP_BELOW_THREE_QUARTERS))
                            mask &= (u8) ~(BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PROTECT)
                                | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SHELL)
                                | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_WALL));
                        if ((mask
                                & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_HASTE)
                                    | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SLOW)))
                            && ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_STOP)]
                                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_STOP))
                                || (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_SLEEP)]
                                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SLEEP))))
                            mask &= (u8) ~(BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_HASTE)
                                | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SLOW));
                        break;
                    case 4:
                        if (!(decision->targeting_flags_2 & AI_TARGET_HP_BELOW_THREE_QUARTERS))
                            mask &= (u8) ~(BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FAITH)
                                | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_REFLECT));
                        break;
                    }
                    if (mask)
                        return AI_ABILITY_EFFECT_USEFUL;
                }
            }
        }
    }
    if (ai->considered_ability.ai_flags.word & AI_ABILITY_CANCEL_STATUS) {
        for (i = 0; i < BATTLE_STATUS_BYTE_COUNT; i++) {
            mask = ai->considered_ability.parameters.bytes.status_infliction[i];
            mask &= unit->status_sets.current[i];
            if (mask) {
                mask ^= mask & unit->status_sets.innate[i];
                if (mask) {
                    if (i >= 3) {
                        for (bit = 0; bit < 8; bit++) {
                            s32 flag = 0x80 >> bit;
                            if (mask & flag) {
                                status = i * 8 + bit;
                                if (!ai_is_status_active_through_delay(ct, unit, status)
                                    || !ai_check_unit_gets_turn_without_status(unit, status))
                                    mask ^= flag;
                            }
                        }
                    }
                    switch (i) {
                    case 1:
                        if ((mask & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SILENCE)) && !decision->silence_mod)
                            mask ^= BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SILENCE);
                        if ((mask & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DARKNESS)) && !decision->evade_mod)
                            mask ^= BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DARKNESS);
                        break;
                    case 2:
                        if ((mask & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG)) && decision->enemy_flag)
                            mask ^= BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG);
                        break;
                    }
                    if (mask)
                        return AI_ABILITY_EFFECT_USEFUL;
                }
            }
        }
    }
    /* Late stat-effect gate: Praise, Accumulate, and Yell only.
     * Odd parity still falls through to the Unequip flag below. */
    if (ai->considered_ability.ai_flags.word & AI_ABILITY_AFFECTS_STATS) {
        ability_id = ai->considered_ability.ability_id;
        if (ability_id != ABILITY_ID_TALK_PRAISE && ability_id != ABILITY_ID_BASIC_SKILL_ACCUMULATE
            && ability_id != ABILITY_ID_GUTS_YELL)
            return AI_ABILITY_EFFECT_USEFUL;
        if (!(rand() & 1))
            return AI_ABILITY_EFFECT_USEFUL;
    }
    if (ai->considered_ability.ai_flags.word & AI_ABILITY_UNEQUIP)
        return AI_ABILITY_EFFECT_USEFUL;
    return AI_ABILITY_EFFECT_NONE;
}
