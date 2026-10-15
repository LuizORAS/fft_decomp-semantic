#include "fft/main.h"
#include "psx/libspu.h"
#include "psx/types.h"

/* Pushes the voice attributes marked in each channel's voice_attr_flags to
 * the SPU, for every MUS record that is playing or owes a full voice update
 * and is not excluded by SUZUKI_MUSIC_STATUS_NO_VOICE_ATTRS, then clears the
 * marks. Voices reserved or pending release by SFX are skipped except for
 * the SFX record.
 *
 * The PITCH, ATTACK and SUSTAIN marks double as the marks set by the LFO,
 * noise and reverb opcodes (main_smd_enable_lfo, main_smd_turn_on_noise,
 * main_smd_enable_reverb), so they also request the pitch-LFO, noise and
 * reverb voice-mask recalculations (g_main_sound_voice_update_requests). The
 * volume/mode call already sets the volume, so VOLUME is dropped after it. */
void main_sound_update_voice_attrs(void) {
    suzuki_music_t* music;
    suzuki_music_channel_t* channel;
    s16 status;
    s32 count;
    u32 allowed;
    u16 flags;
    s32 voice;

    music = g_main_sound_active_music_list;
    while (music != 0) {
        status = music->status;
        if ((status & (SUZUKI_MUSIC_STATUS_FULL_VOICE_UPDATE | SUZUKI_MUSIC_STATUS_PLAYING)) != 0
            && (status & SUZUKI_MUSIC_STATUS_NO_VOICE_ATTRS) == 0) {
            music->status = status & ~SUZUKI_MUSIC_STATUS_FULL_VOICE_UPDATE;
            count = music->channel_count;
            channel = music->channels;
            if ((status & SUZUKI_MUSIC_STATUS_SFX) != 0)
                allowed = -1;
            else
                allowed = ~(g_main_mask_exclusion | g_main_sound_sfx_key_off_voices);
            do {
                if ((channel->flags & SUZUKI_CHANNEL_IN_USE) != 0 && (allowed & channel->voice_mask) != 0) {
                    flags = channel->voice_attr_flags;
                    if (flags != 0) {
                        voice = channel->voice;
                        if ((flags & SUZUKI_VOICE_ATTR_VOLUME_MODE) != 0) {
                            SpuSetVoiceVolumeAttr(voice, channel->spu_volume_left, channel->spu_volume_right,
                                channel->spu_volume_mode_left, channel->spu_volume_mode_right);
                            flags &= ~SUZUKI_VOICE_ATTR_VOLUME;
                        }
                        if ((flags & SUZUKI_VOICE_ATTR_VOLUME) != 0)
                            SpuSetVoiceVolume(voice, channel->spu_volume_left, channel->spu_volume_right);
                        if ((flags & SUZUKI_VOICE_ATTR_PITCH) != 0)
                            SpuSetVoicePitch(voice, channel->spu_pitch);
                        if ((flags & SUZUKI_VOICE_ATTR_ADDRESS) != 0) {
                            SpuSetVoiceStartAddr(voice, channel->start_address);
                            SpuSetVoiceLoopStartAddr(voice, channel->loop_address);
                        }
                        if ((flags & SUZUKI_VOICE_ATTR_ATTACK) != 0)
                            SpuSetVoiceARAttr(voice, channel->attack_rate, channel->attack_mode);
                        if ((flags & SUZUKI_VOICE_ATTR_DECAY) != 0)
                            SpuSetVoiceDR(voice, channel->decay_rate);
                        if ((flags & SUZUKI_VOICE_ATTR_SUSTAIN) != 0)
                            SpuSetVoiceSRAttr(voice, channel->sustain_rate, channel->sustain_mode);
                        if ((flags & SUZUKI_VOICE_ATTR_RELEASE) != 0)
                            SpuSetVoiceRRAttr(voice, channel->release_rate, channel->release_mode);
                        if ((flags & SUZUKI_VOICE_ATTR_SUSTAIN_LEVEL) != 0)
                            SpuSetVoiceSL(voice, channel->sustain_level);
                        if ((flags & SUZUKI_VOICE_ATTR_PITCH) != 0)
                            g_main_sound_voice_update_requests |= SUZUKI_VOICE_MASK_REQUEST_PITCH_LFO;
                        if ((flags & SUZUKI_VOICE_ATTR_ATTACK) != 0)
                            g_main_sound_voice_update_requests |= SUZUKI_VOICE_MASK_REQUEST_NOISE;
                        if ((flags & SUZUKI_VOICE_ATTR_SUSTAIN) != 0)
                            g_main_sound_voice_update_requests |= SUZUKI_VOICE_MASK_REQUEST_REVERB;
                        channel->voice_attr_flags = 0;
                    }
                }
                channel++;
            } while (--count != 0);
        }
        music = music->next;
    }
}
