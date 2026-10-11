#include "fft/main.h"
#include "psx/types.h"

/* SMD opcode 0xC3 "Decay Rate". */
u8* main_smd_decay_time(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    u16 value;

    value = *note_data++;
    channel->voice_attr_flags |= SUZUKI_VOICE_ATTR_DECAY;
    channel->decay_rate = value;
    return note_data;
}
