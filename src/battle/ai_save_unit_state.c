#include "fft/battle_action.h"
#include "fft/battle_ai.h"
#include "fft/battle_status.h"
#include "fft/battle_unit.h"
#include "fft/memory.h"

/*
 * Save baseline unit data, or suspend simulated status and restore baseline status.
 *
 * BASELINE saves selected fields, action records, and team Golem values. Any
 * non-baseline mode saves only the compact status record before restoring the
 * baseline identity, death counter, current status, and Death Sentence CT.
 */
void ai_save_unit_state(ai_snapshot_mode_e mode) {
    ai_data_t* ai = &g_ai_data_base;
    ai_status_snapshot_t* status;
    ai_extended_snapshot_t* snapshot;
    unit_stats_t* unit;
    s32 i;
    s32 id;

    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        id = ai->unit_compact_indices[i];
        if (id == 0xff) {
            continue;
        }
        unit = &g_battle_unit_stats[i];
        if (mode != AI_SNAPSHOT_BASELINE) {
            status = &ai->unit_status_records[id];
        } else {
            status = &g_ai_unit_snapshots[id].status;
        }
        status->entd_slot = unit->entd_slot;
        status->death_counter = unit->death_counter;
        mem_copy_bytes(status->current_status, unit->status_sets.current, sizeof(status->current_status));
        if (mode != AI_SNAPSHOT_BASELINE) {
            status->death_sentence_ct = unit->status_ct[STATUS_CT_INDEX(STATUS_ID_DEATH_SENTENCE)];
            snapshot = &g_ai_unit_snapshots[id];
            unit->entd_slot = snapshot->status.entd_slot;
            unit->death_counter = snapshot->status.death_counter;
            unit->status_ct[STATUS_CT_INDEX(STATUS_ID_DEATH_SENTENCE)]
                = snapshot->status_ct[STATUS_CT_INDEX(STATUS_ID_DEATH_SENTENCE)];
            mem_copy_bytes(
                unit->status_sets.current, snapshot->status.current_status, sizeof(snapshot->status.current_status));
        } else {
            snapshot = (ai_extended_snapshot_t*)status;
            snapshot->team_flags = unit->team_flags;
            mem_copy_bytes(snapshot->status_ct, unit->status_ct, BATTLE_TIMED_STATUS_COUNT);
            mem_copy_bytes(snapshot->equipment, unit->equipment, sizeof(snapshot->equipment));
            snapshot->brave = unit->brave;
            mem_copy_bytes(&snapshot->faith, &unit->faith, 4);
            snapshot->mp = unit->mp;
            mem_copy_bytes(snapshot->base_attributes, unit->base_attributes, 3);
            mem_copy_bytes(snapshot->attributes, unit->attributes, 4);
            mem_copy_bytes(&snapshot->x, &unit->x, 3);
            snapshot->charged_ability_ct = unit->charged_ability_ct;
            mem_copy_bytes(&snapshot->has_turn, &unit->has_turn, 3);
            snapshot->mount_info = unit->mount_info;
            mem_copy_bytes(&snapshot->auto_battle_setting, &unit->auto_battle_setting, 8);
            mem_copy_u16_values(&ai->unit_action_records[id], &unit->action_command, 20);
        }
    }
    if (mode == AI_SNAPSHOT_BASELINE) {
        mem_copy_bytes(ai->team_golem, g_battle_team_golem, sizeof(ai->team_golem));
    }
}
