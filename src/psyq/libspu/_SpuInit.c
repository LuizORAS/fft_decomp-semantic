/* SCUS_942.21 0x800186e4..0x800187db. */
#include "psx/libspu.h"

#include "psx/libetc.h"

/* Start or restart LIBSPU: start LIBETC's interrupts (ResetCallback), initialize the SPU (_spu_init), on a
 * cold start set every voice's centre note to 0xc000, install the DMA handler (SpuStart), turn the reverb off
 * with its settings zeroed and its work area at mode 0's start, and clear the heap, the transfer mode, the key
 * state and the queued requests. SpuInit (`hot` 0) and SpuInitHot (1) call it. */
void _SpuInit(s32 hot) {
    s32 i;
    u16 sample_note;
    ResetCallback();
    _spu_init(hot);
    sample_note = 0xc000;
    if (!hot) {
        for (i = 23; i >= 0; i--) {
            _spu_voice_centerNote[i] = sample_note;
        }
    }
    SpuStart();
    _spu_rev_flag = 0;
    _spu_rev_reserve_wa = 0;
    _spu_rev_attr.mode = 0;
    _spu_rev_attr.depth.left = 0;
    _spu_rev_attr.depth.right = 0;
    _spu_rev_attr.delay = 0;
    _spu_rev_attr.feedback = 0;
    _spu_rev_offsetaddr = _spu_rev_startaddr[0];
    _spu_FsetRXX(PSYQ_SPU_REG_REVERB_START, _spu_rev_offsetaddr, PSYQ_SPU_REGISTER_RAW);
    _spu_AllocBlockNum = 0;
    _spu_AllocLastNum = 0;
    _spu_memList = 0;
    _spu_trans_mode = 0;
    _spu_transMode = 0;
    _spu_keystat = 0;
    _spu_RQmask = 0;
    _spu_RQvoice = 0;
    _spu_env = 0;
}
