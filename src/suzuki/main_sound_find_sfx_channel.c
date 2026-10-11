#include "psx/suzuki.h"
#include "psx/types.h"

/* Stops the SFX channels already playing sound_id, recording their channel
 * and voice masks in g_main_sound_sfx_restart_channels/g_main_sound_sfx_restart_voices for main_sound_start_sfx, then
 * returns the first channel of a free group of voice_count channels,
 * searching down from channel 6 - voice_count in steps of voice_count. When
 * every group is busy it returns the channel with the smallest start tick
 * among those whose priority is at most STEALABLE; best is left unset if none
 * qualifies, as in the target.
 *
 * mask is reused for the search mask: a separate variable changes the
 * register allocation of the stop loop. */
s32 main_sound_find_sfx_channel(s32 sound_id, s32 voice_count) {
    suzuki_music_t* music;
    suzuki_music_channel_t* channel;
    u32 mask;
    u32 channels;
    u32 voices;
    s32 count;
    s32 index;
    u32 group;
    u32 busy;
    u32 oldest;
    s32 best;

    music = g_main_sound_sfx_music;
    voices = 0;
    channels = 0;
    mask = 1;
    count = SUZUKI_SFX_CHANNEL_COUNT;
    channel = music->channels;
    do {
        if (channel->flags & SUZUKI_CHANNEL_IN_USE) {
            if (channel->sound_id == sound_id) {
                channels |= mask;
                voices |= channel->voice_attr.voice;
                channel->flags = 0;
            }
        }
        mask <<= 1;
        channel++;
    } while (--count != 0);

    index = 6 - voice_count;
    group = 0xFFFFFFFF >> (32 - voice_count);
    mask = group << index;
    music = g_main_sound_sfx_music;
    g_main_sound_sfx_restart_channels = channels;
    g_main_sound_sfx_restart_voices = voices;
    channel = &music->channels[index];
    busy = ~channels & music->channel_mask;
    oldest = -1;
    while (busy & mask) {
        if (channel->start_tick < oldest) {
            if (channel->priority <= MAIN_SOUND_SFX_PRIORITY_STEALABLE) {
                oldest = channel->start_tick;
                best = index;
            }
        }
        mask >>= voice_count;
        if (mask < group) {
            index = best;
            break;
        }
        channel -= voice_count;
        index -= voice_count;
    }
    return index;
}
