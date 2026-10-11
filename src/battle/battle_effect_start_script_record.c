#include "fft/battle.h"

/* Allocate an effect state record for a script and reset its per-handler state.
 * Returns the record index, or 0 when none is available. */
s16 battle_effect_start_script_record(u8* script, s16 start_pc, s16 parent_index) {
    s16 index;
    s16 i;
    effect_record_t* record;

    index = battle_effect_alloc_state_record();
    if (index == 0) {
        return 0;
    }
    record = &g_battle_effect_state_records[index];
    record->parent_index = parent_index;
    record->script = script;
    record->pc = start_pc;
    for (i = 0; i < EFFECT_SCRIPT_COUNTER_COUNT; i++) {
        record->counters[i] = 0;
        record->child_indices[i] = 0;
    }
    for (i = 0; i < EFFECT_RECORD_CALLBACK_SLOT_COUNT; i++) {
        record->callbacks[i] = 0;
        record->callback_phase[i] = EFFECT_PHASE_IDLE;
        record->work_slots[i] = 0;
    }
    record->flags = EFFECT_RECORD_FLAG_ALLOCATED;
    record->work_head = 0;
    record->work_count = 0;
    record->frame_counter = -1;
    battle_effect_reset_child_timeline(record, 0);
    battle_effect_reset_parent_timeline(record);
    return index;
}
