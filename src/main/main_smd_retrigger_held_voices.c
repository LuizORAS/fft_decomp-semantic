#include "fft/main.h"
#include "psx/types.h"

/* Marks every in-use channel that holds a fermata and is not muted or
 * flagged 0x10 for retriggering (voice_update_flags bit 0) and returns the voices
 * of those channels.
 *
 * The two single-bit tests are merged by GCC into one word-wide
 * `(flags & 0x101) == 0x101` test (`lw`), which the target contains; a
 * direct 0x101 mask test compiles to `lhu`. */
u32 main_smd_retrigger_held_voices(suzuki_music_t* music) {
    suzuki_music_channel_t* channel;
    s32 count;
    u32 voices;

    count = music->channel_count;
    channel = music->channels;
    voices = 0;
    do {
        if ((channel->flags & SUZUKI_CHANNEL_IN_USE) && (channel->flags & SUZUKI_CHANNEL_SOUNDING)) {
            if ((channel->flags & (SUZUKI_CHANNEL_MUTED | SUZUKI_CHANNEL_NO_RETRIGGER)) == 0) {
                voices |= channel->voice_mask;
                channel->voice_update_flags |= SUZUKI_VOICE_UPDATE_KEY_ON;
            }
        }
        count--;
        channel++;
    } while (count != 0);
    return voices;
}
