#include "psx/suzuki.h"
#include "psx/types.h"

/* SMD opcode 0xE2 "Volume Ramp": ramps the channel volume to a signed
 * target byte over the given number of ticks. It holds no note. */
u8* main_smd_fermata_ramp(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    u16 steps;
    s32 delta;

    steps = note_data[0];
    delta = ((s8)note_data[1] << 24) - channel->volume;
    if (steps != 0 && delta != 0) {
        channel->volume_step_count = steps;
        channel->ramp_flags |= SUZUKI_RAMP_VOLUME;
        channel->volume_step = delta / steps;
    }
    return note_data + 2;
}
