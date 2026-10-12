#include "psx/suzuki.h"
#include "psx/types.h"

/* Stops SFX channels index and index + 1 if they are in use and queues
 * their voices for key-off. No caller is known. */
void suzuki_sound_stop_sfx_channel_pair(s32 index) {
    suzuki_music_t* music;
    suzuki_music_channel_t* channel;
    u32 bit;
    u32 channels;
    u32 voices;
    s32 count;

    bit = 1 << index;
    voices = 0;
    channels = 0;
    count = 2;
    music = g_suzuki_sound_sfx_music;
    channel = &music->channels[index];
    do {
        if (channel->flags & SUZUKI_CHANNEL_IN_USE) {
            channels |= bit;
            channel->flags = 0;
            voices |= channel->voice_attr.voice;
        }
        bit <<= 1;
        channel++;
    } while (--count != 0);
    if (channels != 0) {
        suzuki_sound_release_sfx_voices(music, channels, voices);
    }
}
