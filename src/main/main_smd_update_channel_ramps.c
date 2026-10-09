#include "fft/main.h"
#include "psx/types.h"

/* Per-tick channel pass run before the SMD interpreter. Steps the MUS tempo
 * ramp (opcode 0xA2) and rescales the tempo, then for every channel still
 * inside a note or rest (rest_length != 0) clears the modulation outputs,
 * steps the volume (ramp_flags bit 3), pitch-slide (bit 0, held by bit 1) and
 * balance (bit 4) ramps, and counts down rest_length and note_ticks.
 * voice_update_flags collects the resulting voice work: 0x100 volume/balance,
 * 0x200 pitch, 2 key off. When rest_length reaches 1 on a channel with
 * active bit 0x1000 the release time is forced to 6. */
void main_smd_update_channel_ramps(suzuki_music_t* music, suzuki_music_channel_t* channel, s16 channel_count) {
    u16 count;
    u16 active;
    u16 voice_update_flags;
    u16 flags;

    count = music->tempo_step_count;
    if (count != 0) {
        if (--count != 0)
            music->tempo.raw += music->tempo_step;
        else
            music->tempo.raw = music->tempo_target << 16;
        music->tempo_step_count = count;
        music->scaled_tempo = music->tempo.halves.high * (music->tempo_scale.value >> 16);
    }
    do {
        active = channel->flags;
        if (active != 0) {
            voice_update_flags = channel->voice_update_flags;
            flags = channel->ramp_flags;
            if (channel->rest_length != 0) {
                channel->pitch_modulation = 0;
                channel->volume_modulation = 0;
                channel->balance_modulation = 0;
                if ((flags & SUZUKI_RAMP_VOLUME) != 0) {
                    voice_update_flags |= SUZUKI_VOICE_UPDATE_VOLUME;
                    if (--channel->volume_step_count == 0)
                        flags &= ~SUZUKI_RAMP_VOLUME;
                    channel->volume += channel->volume_step;
                }
                if ((flags & SUZUKI_RAMP_PITCH_SLIDE) != 0) {
                    voice_update_flags |= SUZUKI_VOICE_UPDATE_PITCH;
                    if ((flags & SUZUKI_RAMP_PITCH_SLIDE_HOLD) == 0 && --channel->pitch_slide_count == 0)
                        flags &= ~SUZUKI_RAMP_PITCH_SLIDE;
                    channel->pitch_slide += channel->pitch_slide_step;
                }
                if ((flags & SUZUKI_RAMP_BALANCE) != 0) {
                    voice_update_flags |= SUZUKI_VOICE_UPDATE_VOLUME;
                    if (--channel->balance_step_count == 0)
                        flags &= ~SUZUKI_RAMP_BALANCE;
                    channel->balance += channel->balance_step;
                }
                if (--channel->rest_length == 1 && (active & SUZUKI_CHANNEL_NOTE_FOLLOWS) != 0) {
                    channel->release_time = 6;
                    channel->voice_attr_flags |= SUZUKI_VOICE_ATTR_RELEASE;
                }
                if ((active & (SUZUKI_CHANNEL_KEYED_OFF | SUZUKI_CHANNEL_TIED)) == 0 && --channel->note_ticks == 0) {
                    voice_update_flags |= SUZUKI_VOICE_UPDATE_KEY_OFF;
                    channel->flags |= SUZUKI_CHANNEL_KEYED_OFF;
                }
            }
            channel->voice_update_flags = voice_update_flags;
            channel->ramp_flags = flags;
        }
        channel++;
    } while (--channel_count != 0);
}
