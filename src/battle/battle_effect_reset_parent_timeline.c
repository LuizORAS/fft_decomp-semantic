#include "fft/battle_effect.h"

/* Clear a record's parent-timeline state: the frame, the spawn countdown and count, and the keyframe,
 * frames left and step of each of the 26 tracks. battle_effect_start_script_record and script opcode 2b call
 * it. */
void battle_effect_reset_parent_timeline(battle_effect_record_t* record) {
    s32 index;

    for (index = 0; index < EFFECT_PARENT_TRACK_RESET_SLOT_COUNT; index++) {
        record->timeline.state.parent.step.reset_slots[index] = 0;
        record->timeline.state.parent.remaining.reset_slots[index] = 0;
        record->timeline.state.parent.keyframe.reset_slots[index] = 0;
    }
    record->timeline.state.parent.spawn_countdown = 0;
    record->timeline.frame = 0;
    record->timeline.state.parent.spawned_count = 0;
}
