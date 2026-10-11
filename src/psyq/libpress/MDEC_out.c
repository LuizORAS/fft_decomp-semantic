#include "psx/libpress.h"

/* Start the MDEC output DMA of size 32-bit values into buf. */
void MDEC_out(u32* buf, u32 size) {
    MDEC_out_sync();
    *g_psyq_press_dma_dpcr |= 0x88;
    *g_psyq_press_dma_out_chcr = 0;
    *g_psyq_press_dma_out_madr = (u32)buf;
    *g_psyq_press_dma_out_bcr = ((size >> 5) << 16) | 0x20;
    *g_psyq_press_dma_out_chcr = 0x01000200;
}
