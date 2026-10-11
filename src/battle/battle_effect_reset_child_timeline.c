#include "fft/battle.h"

/* Clears a record's child-timeline state and sets the effect target it plays
 * on. */
void battle_effect_reset_child_timeline(effect_record_t* record, s16 target_index) {
    s32 index;

    index = 0;
    do {
        record->timeline.child.step.values[index] = 0;
        record->timeline.child.remaining.values[index] = 0;
        record->timeline.child.keyframe.values[index] = 0;
        index++;
    } while (index < 13);
    record->timeline_frame = 0;
    record->target_index = target_index;
}
