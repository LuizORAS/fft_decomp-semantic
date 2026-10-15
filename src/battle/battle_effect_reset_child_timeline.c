#include "fft/battle.h"

/* Clears a record's child-timeline state and sets the effect target it plays
 * on. */
void battle_effect_reset_child_timeline(battle_effect_temp_data_t* data, s16 target_index) {
    s32 index;

    index = 0;
    do {
        data->step[index] = 0;
        data->remaining[index] = 0;
        data->keyframe[index] = 0;
        index++;
    } while (index < 13);
    data->frame = 0;
    data->target_index = target_index;
}
