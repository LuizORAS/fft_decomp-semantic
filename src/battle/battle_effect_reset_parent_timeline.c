#include "fft/battle.h"

/* Clear all 26 halfwords in each bank, including the extra reset slots. */
void battle_effect_reset_parent_timeline(effect_record_t* record) {
    s32 index;

    index = 0;
    do {
        record->timeline.parent.step.values[index] = 0;
        record->timeline.parent.remaining.values[index] = 0;
        record->timeline.parent.keyframe.values[index] = 0;
        index++;
    } while (index < 26);
    record->timeline.parent.spawn_countdown = 0;
    record->timeline_frame = 0;
    record->timeline.parent.spawned_count = 0;
}
