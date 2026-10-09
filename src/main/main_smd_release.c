#include "fft/main.h"
#include "psx/types.h"

/* SMD opcode 0xC5 "Release". The release byte lands both in the
 * per-note field at 0x2e and in the envelope block. */
u8* main_smd_release(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    u16 release;

    release = note_data[0];
    channel->voice_attr_flags |= SUZUKI_VOICE_ATTR_RELEASE;
    channel->note_release_time = release;
    channel->release_time = release;
    return note_data + 1;
}
