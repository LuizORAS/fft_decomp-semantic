/* SCUS_942.21 0x80019448..0x800194c3. */
#include "psx/libspu.h"

#include "psx/libapi.h"

void SpuQuit(void) {
    if (_spu_isCalled == 1) {
        _spu_isCalled = 0;
        EnterCriticalSection();
        _spu_transferCallback = 0;
        _spu_IRQCallback = 0;
        _SpuDataCallback(0);
        CloseEvent(_spu_EVdma);
        DisableEvent(_spu_EVdma);
        ExitCriticalSection();
    }
}
