/* SCUS_942.21 0x8001aae0..0x8001ac7b. */
#include "psx/libspu.h"

#include "psx/libapi.h"

/* Zero the reverb work area of `mode` (from its start to the end of SPU RAM; for mode 0 the last 0x80 bytes)
 * by DMA in 1 KB transfers of a zero block, waiting for the DMA event after each, with the transfer callback
 * held back. Returns 0, or -1 for a mode past 9 or one whose work area overlaps allocated SPU RAM.
 * SpuSetReverbModeParam calls it for a mode with 0x100 added. */
s32 SpuClearReverbWorkArea(s32 mode) {
    /* Retail keeps the saved callback in a volatile stack slot and reloads it for each use. */
    volatile SpuTransferCallbackProc callback = 0;
    u32 remaining;
    u32 address;
    s32 more;
    s32 count;
    s32 saved_mode;
    s32 changed_mode = 0;
    if ((u32)mode >= 10 || _SpuIsInAllocateArea_(_spu_rev_startaddr[mode])) {
        return -1;
    }
    if (mode == 0) {
        remaining = 0x10 << _spu_mem_mode_plus;
        address = 0xfff0 << _spu_mem_mode_plus;
    } else {
        remaining = (0x10000 - _spu_rev_startaddr[mode]) << _spu_mem_mode_plus;
        address = _spu_rev_startaddr[mode] << _spu_mem_mode_plus;
    }
    saved_mode = _spu_transMode;
    if (saved_mode == 1) {
        _spu_transMode = 0;
        changed_mode = 1;
    }
    more = 1;
    if (_spu_transferCallback != 0) {
        callback = _spu_transferCallback;
        _spu_transferCallback = 0;
    }
    while (more) {
        if (remaining > 1024) {
            count = 1024;
        } else {
            count = remaining;
            more = 0;
        }
        _spu_t(PSYQ_SPU_DMA_SET_ADDRESS, address);
        _spu_t(PSYQ_SPU_DMA_WRITE);
        _spu_t(PSYQ_SPU_DMA_START_TRANSFER, _spu_zerobuf, count);
        WaitEvent(_spu_EVdma);
        remaining -= 1024;
        address += 1024;
    }
    if (changed_mode) {
        _spu_transMode = saved_mode;
    }
    if (callback != 0) {
        _spu_transferCallback = callback;
    }
    return 0;
}
