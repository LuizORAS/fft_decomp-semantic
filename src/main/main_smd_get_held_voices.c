#include "fft/main.h"
#include "psx/types.h"

/* Returns the voices of every in-use channel that is sounding and is not
 * muted or flagged SUZUKI_CHANNEL_NO_RETRIGGER: the read-only twin of
 * main_smd_retrigger_held_voices, with the same merged bit test. */
u32 main_smd_get_held_voices(suzuki_music_t* music) {
    suzuki_music_channel_t* channel;
    s32 count;
    u32 voices;

    count = music->channel_count;
    channel = music->channels;
    voices = 0;
    do {
        if ((channel->flags & SUZUKI_CHANNEL_IN_USE) && (channel->flags & SUZUKI_CHANNEL_SOUNDING)) {
            if ((channel->flags & (SUZUKI_CHANNEL_MUTED | SUZUKI_CHANNEL_NO_RETRIGGER)) == 0) {
                voices |= channel->voice_attr.voice;
            }
        }
        count--;
        channel++;
    } while (count != 0);
    return voices;
}
