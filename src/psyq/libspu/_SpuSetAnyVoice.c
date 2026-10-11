/* SCUS_942.21 0x80019b80..0x80019d87. */
#include "psx/libspu.h"

u32 _SpuSetAnyVoice(s32 on_off, u32 voice_bit, s32 reg_index_low, s32 reg_index_high) {
    u32 current;
    if (_spu_env & 1) {
        current = ((_spu_RQ[reg_index_high - PSYQ_SPU_REG_KEY_ON_LOW] & 0xff) << 16)
            | _spu_RQ[reg_index_low - PSYQ_SPU_REG_KEY_ON_LOW];
    } else {
        current = ((_spu_RXX->raw[reg_index_high] & 0xff) << 16) | _spu_RXX->raw[reg_index_low];
    }
    switch (on_off) {
    case SPU_ON:
        if (_spu_env & 1) {
            _spu_RQ[reg_index_low - PSYQ_SPU_REG_KEY_ON_LOW] |= voice_bit;
            _spu_RQ[reg_index_high - PSYQ_SPU_REG_KEY_ON_LOW] |= (voice_bit >> 16) & 0xff;
            _spu_RQmask |= 1 << ((reg_index_low - PSYQ_SPU_REG_KEY_OFF_LOW) >> 1);
        } else {
            _spu_RXX->raw[reg_index_low] |= voice_bit;
            _spu_RXX->raw[reg_index_high] |= (voice_bit >> 16) & 0xff;
        }
        current |= voice_bit & SPU_ALLCH;
        break;
    case SPU_OFF:
        if (_spu_env & 1) {
            _spu_RQ[reg_index_low - PSYQ_SPU_REG_KEY_ON_LOW] &= ~voice_bit;
            _spu_RQ[reg_index_high - PSYQ_SPU_REG_KEY_ON_LOW] &= ~((voice_bit >> 16) & 0xff);
            _spu_RQmask |= 1 << ((reg_index_low - PSYQ_SPU_REG_KEY_OFF_LOW) >> 1);
        } else {
            _spu_RXX->raw[reg_index_low] &= ~voice_bit;
            _spu_RXX->raw[reg_index_high] &= ~((voice_bit >> 16) & 0xff);
        }
        current &= ~(voice_bit & SPU_ALLCH);
        break;
    }
    return current & SPU_ALLCH;
}
