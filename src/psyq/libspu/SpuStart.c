/* SCUS_942.21 0x800187dc..0x80018857. */
#include "psx/libspu.h"

#include "psx/libapi.h"
#include "psx/libetc.h"

/* Install LIBSPU's DMA completion handler (_spu_FiDMA on DMA channel 4) and open and enable the SPU's DMA
 * event, once. _SpuInit calls it. */
void SpuStart(void) {
    if (!_spu_isCalled) {
        _spu_isCalled = 1;
        EnterCriticalSection();
        _SpuDataCallback(_spu_FiDMA);
        _spu_EVdma = OpenEvent(HwSPU, EvSpCOMP, EvMdNOINTR, 0);
        EnableEvent(_spu_EVdma);
        ExitCriticalSection();
    }
}
