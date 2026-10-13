#include "fft/battle_action.h"
#include "fft/battle_ai.h"
#include "fft/battle_status.h"
#include "fft/battle_unit.h"
#include "fft/memory.h"

/*
 * Restore baseline unit data or suspended simulation status.
 *
 * BASELINE restores selected baseline fields, action records, and team Golem
 * values. Any non-baseline mode restores only the compact status record.
 */
void ai_restore_unit_state(ai_snapshot_mode_e mode) {
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
        unit->entd_slot = status->entd_slot;
        unit->death_counter = status->death_counter;
        ai_copy_bytes(unit->status_sets.current, status->current_status, sizeof(status->current_status));
        if (mode != AI_SNAPSHOT_BASELINE) {
            unit->status_ct[STATUS_CT_INDEX(STATUS_ID_DEATH_SENTENCE)] = status->death_sentence_ct;
        } else {
            snapshot = (ai_extended_snapshot_t*)status;
            unit->team_flags = snapshot->team_flags;
            ai_copy_bytes(unit->status_ct, snapshot->status_ct, BATTLE_TIMED_STATUS_COUNT);
            ai_copy_bytes(unit->equipment, snapshot->equipment, sizeof(snapshot->equipment));
            unit->brave = snapshot->brave;
            ai_copy_bytes(&unit->faith, &snapshot->faith, 4);
            unit->mp = snapshot->mp;
            ai_copy_bytes(unit->base_attributes, snapshot->base_attributes, 3);
            ai_copy_bytes(unit->attributes, snapshot->attributes, 4);
            ai_copy_bytes(&unit->x, &snapshot->x, 3);
            unit->charged_ability_ct = snapshot->charged_ability_ct;
            ai_copy_bytes(&unit->has_turn, &snapshot->has_turn, 3);
            unit->mount_info = snapshot->mount_info;
            ai_copy_bytes(&unit->auto_battle_setting, &snapshot->auto_battle_setting, 8);
            ai_copy_u16_values(&unit->action_command, &ai->unit_action_records[id], 20);
        }
    }
    if (mode == AI_SNAPSHOT_BASELINE) {
        ai_copy_bytes(g_battle_team_golem, ai->team_golem, sizeof(ai->team_golem));
    }
}
