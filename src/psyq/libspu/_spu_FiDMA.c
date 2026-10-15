/* SCUS_942.21 0x80018cb8..0x80018d87. */
#include "psx/libspu.h"

#include "psx/libapi.h"
#include "psx/libetc.h"

/* LIBSPU's DMA completion handler: after a write wait a moment (three _spu_Fw1ts), leave the SPU's
 * transfer mode and wait for it to clear (at most 3840 polls), then call the transfer callback or, with none,
 * deliver the SPU's DMA event (which SpuClearReverbWorkArea waits on). */
void _spu_FiDMA(void) {
    u32 wait;
    if (g_psyq_spu_dma_direction == 0) {
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
    }
    _spu_RXX->rxx.control &= 0xffcf;
    wait = 0;
    while (_spu_RXX->rxx.control & 0x30) {
        if (++wait > 3840) {
            break;
        }
    }
    if (_spu_transferCallback != 0) {
        _spu_transferCallback();
    } else {
        DeliverEvent(HwSPU, EvSpCOMP);
    }
}
