#include "fft/battle_effect.h"

/* Clear all 26 16-bit values in each bank, including the extra reset slots. */
void battle_effect_reset_parent_timeline(effect_record_t* record) {
    s32 index;

    index = 0;
    do {
        record->timeline.state.parent.step.reset_slots[index] = 0;
        record->timeline.state.parent.remaining.reset_slots[index] = 0;
        record->timeline.state.parent.keyframe.reset_slots[index] = 0;
        index++;
    } while (index < BATTLE_EFFECT_PARENT_TRACK_RESET_SLOT_COUNT);
    record->timeline.state.parent.spawn_countdown = 0;
    record->timeline.frame = 0;
    record->timeline.state.parent.spawned_count = 0;
}
