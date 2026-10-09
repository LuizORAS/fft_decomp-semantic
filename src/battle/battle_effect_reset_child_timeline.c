#include "fft/battle.h"

void battle_effect_reset_child_timeline(battle_effect_temp_data_t* data, s16 value) {
    s32 index;

    index = 0;
    do {
        data->step[index] = 0;
        data->remaining[index] = 0;
        data->keyframe[index] = 0;
        index++;
    } while (index < 13);
    data->frame = 0;
    data->target_index = value;
}
