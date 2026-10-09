#include "fft/main.h"
#include "psx/types.h"

/* SMD opcode 0xC9 "Sustain Mode". */
u8* main_smd_sustain_mode(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    u32 mode;

    mode = note_data[0];
    channel->voice_attr_flags |= SUZUKI_VOICE_ATTR_SUSTAIN;
    channel->sustain_mode = mode;
    return note_data + 1;
}
