#include "fft/main.h"
#include "psx/types.h"

/* SMD opcode 0x80 "Rest". Note flag bit 10 (0x400) and voice_update_flags
 * bit 1 mark the pending rest for the note dispatcher. */
u8* main_smd_rest(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    channel->rest_length = note_data[0];
    channel->flags |= 0x400;
    channel->voice_update_flags |= 2;
    return note_data + 1;
}
