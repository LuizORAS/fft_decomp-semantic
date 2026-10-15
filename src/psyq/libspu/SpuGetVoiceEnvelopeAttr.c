/* SCUS_942.21 0x8001aef4..0x8001af63. */
#include "psx/libspu.h"

/* Read voice `v_num`'s envelope level into `envx` and its key state into `key_stat`: 1 keyed on and
 * sounding, 3 keyed on and silent, 2 keyed off and still sounding, 0 off and silent; the state is stored as a
 * halfword (QUIRKS.md). main_smd_end_track calls it. */
void SpuGetVoiceEnvelopeAttr(s32 v_num, s32* key_stat, s16* envx) {
    u16 envelope = _spu_RXX->raw[v_num << 3 | 6];
    *envx = envelope;
    /* Retail writes only a halfword despite the SDK's long-pointer ABI. */
    if (_spu_keystat & (1 << v_num)) {
        *(s16*)key_stat = (s16)envelope > 0 ? 1 : 3;
    } else {
        *(s16*)key_stat = (s16)envelope > 0 ? 2 : 0;
    }
}
