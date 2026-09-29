/* SCUS_942.21 0x80018cb8..0x80018d87. */
#include "psx/libspu.h"

#include "psx/libapi.h"
#include "psx/libetc.h"

void _spu_FiDMA(void) {
    /* Volatile local alias keeps the retail callback reload with symbolic addressing. */
    extern void (*volatile callback)(void) __asm__("_spu_transferCallback");
    u32 wait;
    volatile psyq_spu_registers_t* registers;
    if (g_psyq_spu_dma_direction == 0) {
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
    }
    wait = 0;
    registers = _spu_RXX;
    registers->control &= 0xffcf;
    while (registers->control & 0x30) {
        if (++wait > 3840) {
            break;
        }
    }
    if (_spu_transferCallback != 0) {
        callback();
    } else {
        DeliverEvent(HwSPU, EvSpCOMP);
    }
}
