/* SCUS_942.21 0x80019448..0x800194c3. */
#include "psx/libspu.h"

#include "psx/libapi.h"

/* Undo SpuStart, once: clear the transfer and SPU interrupt callbacks, remove the DMA handler, then close
 * and disable the DMA event, in a critical section. main_system_stop_display_and_audio_for_game_reset calls
 * it. */
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
