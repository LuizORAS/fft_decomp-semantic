/* SCUS_942.21 0x8001aae0..0x8001ac7b. */
#include "psx/libspu.h"

#include "psx/libapi.h"

s32 SpuClearReverbWorkArea(s32 mode) {
    /* Volatile keeps the saved callback in the original stack slot. */
    volatile SpuTransferCallbackProc callback = 0;
    extern void (*volatile current_callback)(void) __asm__("_spu_transferCallback");
    u32 remaining;
    u32 address;
    s32 more;
    /* This view keeps the handle load before the transfer-count update. */
    extern volatile s32 completion_event __asm__("_spu_EVdma");
    s32 event;
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
    {
        SpuTransferCallbackProc active_callback = current_callback;
        more = 1;
        if (active_callback != 0) {
            callback = current_callback;
            current_callback = 0;
        }
    }
    do {
        mode = remaining;
        if (remaining > 1024) {
            mode = 1024;
        } else {
            more = 0;
        }
        _spu_t(PSYQ_SPU_DMA_SET_ADDRESS, address);
        _spu_t(PSYQ_SPU_DMA_WRITE);
        _spu_t(PSYQ_SPU_DMA_START_TRANSFER, _spu_zerobuf, mode);
        event = completion_event;
        /* The barrier retains the event load before the address/count updates. */
        __asm__ volatile("");
        remaining -= 1024;
        address += 1024;
        WaitEvent(event);
    } while (more);
    if (changed_mode) {
        _spu_transMode = saved_mode;
    }
    if (callback != 0) {
        _spu_transferCallback = callback;
    }
    return 0;
}
