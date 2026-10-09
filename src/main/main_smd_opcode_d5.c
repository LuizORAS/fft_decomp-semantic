#include "fft/main.h"
#include "psx/types.h"

/* SMD opcode 0xD5: toggles bit 1 of ramp_flags. */
u8* main_smd_opcode_d5(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    channel->ramp_flags ^= SUZUKI_RAMP_PITCH_SLIDE_HOLD;
    return note_data;
}
