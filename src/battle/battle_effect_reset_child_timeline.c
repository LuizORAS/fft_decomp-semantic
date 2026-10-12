#include "fft/battle_effect.h"

/* Clears a record's child-timeline state and sets the effect target it plays
 * on. */
void battle_effect_reset_child_timeline(effect_record_t* record, s16 target_index) {
    s32 index;

    index = 0;
    do {
        record->timeline.state.child.step.values[index] = 0;
        record->timeline.state.child.remaining.values[index] = 0;
        record->timeline.state.child.keyframe.values[index] = 0;
        index++;
    } while (index < 13);
    record->timeline.frame = 0;
    record->timeline.target_index = target_index;
}
