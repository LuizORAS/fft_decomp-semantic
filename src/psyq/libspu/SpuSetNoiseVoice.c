/* SCUS_942.21 0x80019b5c..0x80019b7f. */
#include "psx/libspu.h"

/* Turn noise on (1) or off (0) for the voices in `voice_bit` and return the voices that have it
 * (_SpuSetAnyVoice). main_sound_calculate_noise_voices calls it. */
u32 SpuSetNoiseVoice(s32 on_off, u32 voice_bit) {
    return _SpuSetAnyVoice(on_off, voice_bit, 0xca, 0xcb);
}
