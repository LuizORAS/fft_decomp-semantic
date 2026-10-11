#include "psx/suzuki.h"
#include "psx/types.h"

/* SMD opcode 0xD3 "Shift Pitch Offset Word": adds a signed 16-bit delta in
 * 1/256 semitone units to the channel pitch offset, carried as a signed high
 * byte followed by an unsigned low byte. */
u8* main_smd_add_pitch_offset_wide(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    s8 high;
    u8 low;

    high = note_data[0];
    low = note_data[1];
    channel->pitch_offset += low + (high << 8);
    channel->voice_update_flags |= SUZUKI_VOICE_UPDATE_PITCH;
    return note_data + 2;
}
