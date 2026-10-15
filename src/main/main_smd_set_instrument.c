#include "fft/main.h"
#include "psx/types.h"

/* Loads instrument entry n of the channel's waveset into the channel:
 * sample and loop addresses, ADSR rates and modes, sustain level and
 * fine_tune. An SFX or immediate-instrument channel gets every voice
 * attribute marked for update at once; otherwise the next note marks them
 * (SUZUKI_CHANNEL_INSTRUMENT_PENDING). The entry index is the instrument
 * number sign-extended from 16 bits, and the loop address is start +
 * loop_offset without the waveset's SPU base that start_address gets.
 *
 * Most music channels on the disc select instrument 0xff before their End
 * Bar. WAVESET.WD has 176 instruments and only its 0xb30-byte header is
 * kept on the heap, so that entry is read 0x4f0 bytes past the copy. */
void main_smd_set_instrument(s16 instrument, suzuki_music_channel_t* channel) {
    suzuki_instrument_t* entry;
    suzuki_waveset_t* waveset;

    channel->instrument = instrument;
    waveset = channel->waveset;
    entry = &waveset->instruments[instrument];
    channel->start_address = entry->start + waveset->spu_address;
    channel->loop_address = entry->loop_offset + entry->start;
    channel->attack_mode = entry->attack_mode;
    channel->sustain_mode = entry->sustain_mode;
    channel->release_mode = entry->release_mode;
    channel->attack_rate = entry->attack_rate;
    channel->decay_rate = entry->decay_rate;
    channel->sustain_rate = entry->sustain_rate;
    channel->release_rate = channel->note_release_rate = entry->release_rate;
    channel->sustain_level = entry->sustain_level;
    channel->fine_tune = entry->fine_tune;
    if (channel->flags & (SUZUKI_CHANNEL_SFX | SUZUKI_CHANNEL_IMMEDIATE_INSTRUMENT)) {
        channel->voice_update_flags |= SUZUKI_VOICE_UPDATE_VOLUME | SUZUKI_VOICE_UPDATE_PITCH;
        channel->voice_attr_flags |= SUZUKI_VOICE_ATTR_ALL;
    } else {
        channel->flags |= SUZUKI_CHANNEL_INSTRUMENT_PENDING;
    }
}
