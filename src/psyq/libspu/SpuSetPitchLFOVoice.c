/* SCUS_942.21 0x8001b070..0x8001b093. */
#include "psx/libspu.h"

u32 SpuSetPitchLFOVoice(s32 on_off, u32 voice_bit) {
    return _SpuSetAnyVoice(on_off, voice_bit, 0xc8, 0xc9);
}
