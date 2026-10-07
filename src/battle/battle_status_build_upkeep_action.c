#include "fft/battle.h"
#include "psx/libc.h"
#include "psx/types.h"

/* The turn clock's status upkeep for one unit, staged in its action record. A dead unit with Reraise
 * that is not undead revives with max HP / 10 (returns 2). While executing, a dead unit counts down
 * its death counter (immortal and formation units never run out); when it runs out, an undead unit
 * revives on a coin flip with 1 to max HP (2), otherwise the unit becomes a Crystal, on a roll of 1/2
 * (3/4 for a unit that started player-controlled), or else a Treasure (1). Death Sentence counts down
 * and at 0 ends, inflicting Dead unless the unit is undead (3). Defending ends, and a Brave below 10
 * gains 1. Returns 2 when anything else changed, 0 when nothing did. */
s32 battle_status_build_upkeep_action(s32 unit_id, battle_stats_t* unit) {
    battle_action_data_t* action = &unit->action;
    s32 roll;
    s32 mask;
    u8 counter;

    battle_action_clear_current_data(action);
    roll = rand();
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
            & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_UNDEAD)))
            == BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD)
        && (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_RERAISE)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_RERAISE))) {
        action->status_removal[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
            = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD);
        action->status_removal[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_RERAISE)]
            = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_RERAISE);
        action->attack_type = BATTLE_ACTION_TYPE_HP_HEALING | BATTLE_ACTION_TYPE_STATUS_CHANGE;
        action->hp_healing = unit->max_hp / 10;
        return 2;
    }
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD))
        && g_battle_action_state == BATTLE_ACTION_STATE_EXECUTE) {
        if (unit->team_flags & BATTLE_TEAM_FLAG_IMMORTAL) {
            return 0;
        }
        if (unit->unit_flags & (UNIT_FLAG_SAVE_FORMATION | UNIT_FLAG_LOAD_FORMATION)) {
            return 0;
        }
        counter = unit->death_counter;
        counter -= 1;
        if (counter != 0xff) {
            unit->death_counter = counter;
            return 0;
        }
        if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_UNDEAD)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_UNDEAD))
            && (roll & 1)) {
            action->status_removal[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
                = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD);
            action->hp_healing = ((rand() * unit->max_hp) / 0x8000) + 1;
            action->attack_type = BATTLE_ACTION_TYPE_HP_HEALING | BATTLE_ACTION_TYPE_STATUS_CHANGE;
            return 2;
        }
        mask = 0x10;
        if (unit->initial_team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) {
            mask = 0x110;
        }
        if (roll & mask) {
            action->status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRYSTAL)]
                = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRYSTAL);
            action->attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
            return 1;
        }
        action->status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TREASURE)]
            = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TREASURE);
        action->attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
        return 1;
    }
    if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEATH_SENTENCE)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEATH_SENTENCE)) {
        counter = unit->status_ct[BATTLE_STATUS_CT_INDEX(BATTLE_STATUS_ID_DEATH_SENTENCE)];
        counter -= 1;
        if (counter == 0) {
            action->status_removal[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEATH_SENTENCE)]
                = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEATH_SENTENCE);
            action->attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
            if (!(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_UNDEAD)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_UNDEAD))) {
                action->status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
                    = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD);
                return 3;
            }
        } else {
            unit->status_ct[BATTLE_STATUS_CT_INDEX(BATTLE_STATUS_ID_DEATH_SENTENCE)] = counter;
        }
    }
    if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEFENDING)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEFENDING)) {
        action->status_removal[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEFENDING)]
            = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEFENDING);
        action->attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
    }
    if (unit->brave < 10) {
        action->brave_change = BATTLE_ACTION_STAT_CHANGE_INCREASE | 1;
        action->attack_type |= BATTLE_ACTION_TYPE_PSEUDO_STATUS;
    }
    return (action->attack_type != 0) * 2;
}
