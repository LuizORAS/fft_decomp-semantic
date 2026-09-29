/* SCUS_942.21 0x8001aabc..0x8001aadf. */
#include "psx/libspu.h"

u32 SpuSetReverbVoice(s32 on_off, u32 voice_bit) {
    return _SpuSetAnyVoice(on_off, voice_bit, 0xcc, 0xcd);
}
