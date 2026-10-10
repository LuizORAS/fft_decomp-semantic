#include "fft/main.h"
#include "psx/types.h"

/* SMD opcode 0xD2: add to the channel pitch offset in
 * 1/32 semitone units (8 of 256). Fine twin of main_smd_opcode_d1. */
u8* main_smd_opcode_d2(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    channel->pitch_offset += (s8)note_data[0] << 3;
    channel->voice_update_flags |= SUZUKI_VOICE_UPDATE_PITCH;
    return note_data + 1;
}
