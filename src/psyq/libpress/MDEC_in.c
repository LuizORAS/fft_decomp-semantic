#include "psx/libpress.h"

/* Send an MDEC command word and start DMA of the following size words. */
void MDEC_in(u32* buf, u32 size) {
    MDEC_in_sync();
    *g_psyq_press_dma_dpcr |= 0x88;
    *g_psyq_press_dma_in_madr = (u32)(buf + 1);
    *g_psyq_press_dma_in_bcr = ((size >> 5) << 16) | 0x20;
    *g_psyq_press_mdec_command = buf[0];
    *g_psyq_press_dma_in_chcr = 0x01000201;
}
