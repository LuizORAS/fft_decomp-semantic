#include "fft/main.h"
#include "psx/types.h"

/* Loads instrument entry n of the channel's waveset into the channel:
 * sample and loop addresses, ADSR rates and modes, sustain level and
 * fine_tune. A channel with note flag 0x4 or 0x8 gets every voice attribute
 * marked for update (voice_update_flags 0x300, voice_attr_flags 0x1ff); otherwise it is
 * flagged 0x8000. The entry index is the instrument number sign-extended
 * from 16 bits, and the loop address is start + loop_offset without the
 * waveset's SPU base that start_address gets. */
void main_smd_set_instrument(s32 instrument, suzuki_music_channel_t* channel) {
    suzuki_instrument_t* entry;
    suzuki_waveset_t* waveset;

    channel->instrument = instrument;
    waveset = channel->waveset;
    entry = &waveset->instruments[(s16)instrument];
    channel->start_address = entry->start + waveset->spu_address;
    channel->loop_address = entry->loop_offset + entry->start;
    channel->attack_mode = entry->attack_mode;
    channel->sustain_mode = entry->sustain_mode;
    channel->release_mode = entry->release_mode;
    channel->attack_time = entry->attack_rate;
    channel->decay_time = entry->decay_rate;
    channel->sustain_time = entry->sustain_rate;
    channel->release_time = channel->note_release_time = entry->release_rate;
    channel->sustain_level = entry->sustain_level;
    channel->fine_tune = entry->fine_tune;
    if (channel->flags & (SUZUKI_CHANNEL_SFX | SUZUKI_CHANNEL_IMMEDIATE_INSTRUMENT)) {
        channel->voice_update_flags |= SUZUKI_VOICE_UPDATE_VOLUME | SUZUKI_VOICE_UPDATE_PITCH;
        channel->voice_attr_flags |= SUZUKI_VOICE_ATTR_ALL;
    } else {
        channel->flags |= SUZUKI_CHANNEL_INSTRUMENT_PENDING;
    }
}
