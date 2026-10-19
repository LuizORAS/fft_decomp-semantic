/* SCUS_942.21 0x8001aef4..0x8001af63. */
#include "psx/libspu.h"

/* Read voice `v_num`'s envelope level into `envx` and its key state into `key_stat`: 1 keyed on and
 * sounding, 3 keyed on and silent, 2 keyed off and still sounding, 0 off and silent; the state is stored as a
 * 16-bit value (QUIRKS.md). suzuki_smd_end_track calls it. */
void SpuGetVoiceEnvelopeAttr(s32 v_num, s32* key_stat, s16* envx) {
    s16 envelope = _spu_RXX->raw[v_num << 3 | 6];
    *envx = envelope;
    /* Only the low 16 bits of the s32 key_stat output are written. */
    if (_spu_keystat & SPU_KEYCH(v_num)) {
        *(s16*)key_stat = envelope > 0 ? 1 : 3;
    } else {
        *(s16*)key_stat = envelope > 0 ? 2 : 0;
    }
}
