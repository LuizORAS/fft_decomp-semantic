/* SCUS_942.21 0x80018d88..0x80018e43. */
#include "psx/libspu.h"

void _spu_Fr_(void* destination, u16 address, u32 blocks) {
    _spu_RXX->transfer_address = address;
    _spu_Fw1ts();
    _spu_Fw1ts();
    _spu_RXX->control |= 0x30;
    /* Without this barrier GCC fills the preceding volatile load delay with the shift. */
    __asm__ volatile("");
    blocks <<= 16;
    _spu_Fw1ts();
    _spu_Fw1ts();
    _spu_FsetDelayW();
    {
        /* Unpinned command uses v1 and swaps the retail command/pointer registers. */
        register u32 command __asm__("$4") = 0x01000200;
        /* Without the tie the low constant half moves into the DMA-pointer load delay. */
        __asm__("" : "=r"(command) : "0"(command));
        *g_psyq_spu_dma_address_register = (u32)destination;
        *g_psyq_spu_dma_block_register = blocks | 0x10;
        g_psyq_spu_dma_direction = 1;
        *g_psyq_spu_dma_control_register = command;
    }
}
