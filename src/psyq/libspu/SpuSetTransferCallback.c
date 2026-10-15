/* SCUS_942.21 0x8001b04c..0x8001b06f. */
#include "psx/libspu.h"

/* Set the routine called when a transfer ends, in place of the DMA event, and return the previous one.
 * SuzukiSPUInitialiser installs the driver's (main_smd_spu_transfer_callback). */
SpuTransferCallbackProc SpuSetTransferCallback(SpuTransferCallbackProc func) {
    SpuTransferCallbackProc previous = _spu_transferCallback;
    if (func != previous) {
        _spu_transferCallback = func;
    }
    return previous;
}
