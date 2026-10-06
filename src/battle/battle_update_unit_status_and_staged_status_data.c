#include "fft/battle.h"

/* Snapshot battle unit unit_id into the event staging record (statuses, counts, charged CT,
 * death counter, HP, team) and strip the statuses an event hides; returns 0 when the unit is already
 * staged, else 1. With script variable 0x1fd set (the PRE_BATTLE masks) every status goes except
 * Crystal, Dead, Invite, Cursed, Treasure and Critical, and monsters, Altima and the undead jobs also
 * keep Undead and Float; otherwise (BATTLE_STARTED) Undead, Jump, Petrify, Darkness, Confusion,
 * Transparent, Chicken, Frog, Haste, Slow, Charm, Sleep and Death Sentence go. The stripped set is
 * kept for the restore, a lost Jump (1) or Float (2) is noted in the staging flags, a charmed unit
 * returns to its initial team, and the removal runs on the main stack
 * (battle_status_apply_unit_action_removal).
 *
 * Each mask branch repeats the whole store tail; cross-jumping merges the copies, and the repeated
 * tail uses give the strength-reduced status walker the target's s1 over the stats pointer. */
s32 battle_update_unit_status_and_staged_status_data(s32 unit_id) {
    battle_stats_t* stats;
    s32 i;
    s32 value;
    unit_status_staging_t* st;

    if (g_battle_unit_status_staging_data->state[unit_id] == 0) {
        stats = battle_unit_get_stats_from_battle_id(unit_id);
        for (i = 0; i < BATTLE_STATUS_BYTE_COUNT; i++) {
            g_battle_unit_status_staging_data->innate[unit_id][i] = stats->status_sets.innate[i];
            g_battle_unit_status_staging_data->inflicted[unit_id][i] = stats->inflicted_status[i];
            g_battle_unit_status_staging_data->current[unit_id][i] = stats->status_sets.current[i];
        }
        for (i = 0; i < BATTLE_TIMED_STATUS_COUNT; i++) {
            g_battle_unit_status_staging_data->status_ct[unit_id][i] = stats->status_ct[i];
        }
        g_battle_unit_status_staging_data->charged_ability_ct[unit_id] = stats->charged_ability_ct;
        g_battle_unit_status_staging_data->death_counter[unit_id] = stats->death_counter;
        g_battle_unit_status_staging_data->hp[unit_id] = stats->hp;
        g_battle_unit_status_staging_data->team_flags[unit_id] = stats->team_flags;
        for (i = 0; i < BATTLE_STATUS_BYTE_COUNT; i++) {
            if (battle_script_get_variable(EVENT_SCRIPT_VAR_PENDING_STAGED_STATUS) != 0) {
                if ((stats->primary_skillset >= SKILLSET_ID_MONSTER_FIRST && stats->primary_skillset < SKILLSET_ID_END)
                    || stats->character_identity == CHARACTER_IDENTITY_ALTIMA_FIRST_FORM
                    || stats->character_identity == CHARACTER_IDENTITY_ALTIMA_SECOND_FORM
                    || stats->job_id == JOB_ID_KNIGHT_UNDEAD || stats->job_id == JOB_ID_ARCHER_UNDEAD
                    || stats->job_id == JOB_ID_WIZARD_UNDEAD || stats->job_id == JOB_ID_TIME_MAGE_UNDEAD
                    || stats->job_id == JOB_ID_ORACLE_UNDEAD || stats->job_id == JOB_ID_SUMMONER_UNDEAD) {
                    value = stats->status_sets.current[i];
                    value &= g_battle_event_status_masks[BATTLE_EVENT_STATUS_MASK_PRE_BATTLE_UNDEAD][i];
                    st = g_battle_unit_status_staging_data;
                    stats->action.status_removal[i] = value;
                    st->added[unit_id][i] = value;
                    st->removed[unit_id][i] = value;
                } else {
                    value = stats->status_sets.current[i];
                    value &= g_battle_event_status_masks[BATTLE_EVENT_STATUS_MASK_PRE_BATTLE][i];
                    st = g_battle_unit_status_staging_data;
                    stats->action.status_removal[i] = value;
                    st->added[unit_id][i] = value;
                    st->removed[unit_id][i] = value;
                }
            } else {
                value = stats->status_sets.current[i];
                value &= g_battle_event_status_masks[BATTLE_EVENT_STATUS_MASK_BATTLE_STARTED][i];
                st = g_battle_unit_status_staging_data;
                stats->action.status_removal[i] = value;
                st->added[unit_id][i] = value;
                st->removed[unit_id][i] = value;
            }
        }
        if (g_battle_unit_status_staging_data->removed[unit_id][BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_JUMP)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_JUMP)) {
            g_battle_unit_status_staging_data->flags |= 1;
        }
        if (g_battle_unit_status_staging_data->removed[unit_id][BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FLOAT)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FLOAT)) {
            g_battle_unit_status_staging_data->flags |= 2;
        }
        if (stats->action.status_removal[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARM)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARM)) {
            stats->team_flags = stats->initial_team_flags;
        }
        g_battle_thread_call_target = (void (*)(void))battle_status_apply_unit_action_removal;
        battle_thread_call_on_main_stack(stats);
        g_battle_unit_status_staging_data->state[unit_id] = 1;
        return 1;
    }
    return 0;
}
