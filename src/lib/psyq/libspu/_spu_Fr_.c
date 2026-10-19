/* SCUS_942.21 0x80018d88..0x80018e43. */
#include "psx/libspu.h"

/* Start a DMA read of `blocks` 64-byte blocks from SPU address `address` (in eight-byte units) into
 * `destination`: set the address and the read mode, set the bus delay and start DMA channel 4.
 * SpuReadDecodedData reads the capture buffers with it. */
void _spu_Fr_(void* destination, u16 address, u32 blocks) {
    _spu_RXX->rxx.transfer_address = address;
    _spu_Fw1ts();
    _spu_Fw1ts();
    _spu_RXX->rxx.control |= 0x30;
    _spu_Fw1ts();
    _spu_Fw1ts();
    _spu_FsetDelayW();
    *g_psyq_spu_dma_address_register = (u32)destination;
    *g_psyq_spu_dma_block_register = (blocks << 16) | 0x10;
    g_psyq_spu_dma_direction = 1;
    *g_psyq_spu_dma_control_register = 0x01000200;
}
