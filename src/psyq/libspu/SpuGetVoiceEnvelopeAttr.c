/* SCUS_942.21 0x8001aef4..0x8001af63. */
#include "psx/libspu.h"

void SpuGetVoiceEnvelopeAttr(s32 v_num, s32* key_stat, s16* envx) {
    /* A combined byte offset preserves the ori omitted by typed field indexing. */
    volatile u16* pointer = (u16*)((u8*)_spu_RXX + ((v_num << 4) | 0x0c));
    u16 envelope = *pointer;
    *envx = envelope;
    /* Retail writes only a halfword despite the SDK's long-pointer ABI. */
    if (_spu_keystat & (1 << v_num)) {
        *(s16*)key_stat = (s16)envelope > 0 ? 1 : 3;
    } else {
        *(s16*)key_stat = (s16)envelope > 0 ? 2 : 0;
    }
}
