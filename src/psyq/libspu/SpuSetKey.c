/* SCUS_942.21 0x8001acf0..0x8001aef3. */
#include "psx/libspu.h"

void SpuSetKey(s32 on_off, u32 voice_bit) {
    /* The low/high pins preserve the retail a2/a3 copies. */
    u32 high;
    register u32 low_half __asm__("$6");
    register u32 high_half __asm__("$7");
    /* These views retain the CPU-state accesses after the preceding queued writes. */
    extern volatile u32 queued_flags __asm__("_spu_RQmask");
    extern volatile u32 queued_keys __asm__("_spu_RQvoice");
    voice_bit &= 0xffffff;
    low_half = voice_bit;
    high = voice_bit >> 16;
    high_half = high;
    switch (on_off) {
    case 1:
        if (_spu_env & 1) {
            _spu_RQ[0] = low_half;
            _spu_RQ[1] = high_half;
            queued_flags |= 1;
            queued_keys |= voice_bit;
            if (_spu_RQ[2] & voice_bit) {
                _spu_RQ[2] &= ~voice_bit;
            }
            if (_spu_RQ[3] & high) {
                u32 clear = ~high;
                u32 bits = _spu_RQ[3];
                bits &= clear;
                _spu_RQ[3] = bits;
            }
        } else {
            _spu_RXX->key_on_low = low_half;
            _spu_RXX->key_on_high = high_half;
            _spu_keystat |= voice_bit;
        }
        break;
    case 0:
        if (_spu_env & 1) {
            _spu_RQ[2] = low_half;
            _spu_RQ[3] = high_half;
            queued_flags |= 1;
            queued_keys &= ~voice_bit;
            if (_spu_RQ[0] & voice_bit) {
                _spu_RQ[0] &= ~voice_bit;
            }
            if (_spu_RQ[1] & high) {
                u32 clear = ~high;
                u32 bits = _spu_RQ[1];
                bits &= clear;
                _spu_RQ[1] = bits;
            }
        } else {
            _spu_RXX->key_off_low = low_half;
            _spu_RXX->key_off_high = high_half;
            _spu_keystat &= ~voice_bit;
        }
        break;
    }
}
