#include "fft/main.h"
#include "psx/types.h"

/* ORs mask into voice_attr_flags of every channel that is in use
 * (callers pass 0x7000 / 0x71ff). */
void main_smd_set_voice_attr_flags_all_channels(suzuki_music_t* music, u16 mask) {
    suzuki_music_channel_t* channel;
    u16 voice_attr_flags;
    s32 count;

    channel = music->channels;
    count = music->channel_count;
    do {
        if (channel->flags != 0) {
            voice_attr_flags = channel->voice_attr_flags; /* temp keeps `or rd,flags,mask` operand order */
            channel->voice_attr_flags = voice_attr_flags | mask;
        }
        channel++;
    } while (--count != 0);
}
