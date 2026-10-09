#include "psx/libc.h"
#include "psx/libpress.h"
#include "psx/types.h"

/* Dump the MDEC status and DMA state to the console, then force the decoder
 * back into its idle configuration. */
s32 timeout(const char* tag) {
    u32 status;
    s32 result;
    /* Unpinned, the `result = 0` set sinks below the MDEC reset and the reset
     * uses $v0/$v1 instead of the target's $v1/$a0. */
    register u32 decoder_status __asm__("$3");
    register u32* decoder_control __asm__("$4");

    printf(g_psyq_press_timeout_format, tag);
    status = *g_psyq_press_mdec_status;
    printf(g_psyq_press_timeout_dma_format, (*g_psyq_press_dma_in_chcr >> 24) & 1,
        (*g_psyq_press_dma_out_chcr >> 24) & 1, *g_psyq_press_dma_in_madr, *g_psyq_press_dma_out_madr);
    printf(g_psyq_press_timeout_fifo_format, ~status >> 31, (status >> 30) & 1, (status >> 29) & 1, (status >> 28) & 1,
        (status >> 27) & 1, (status >> 25) & 1, (status >> 23) & 1);
    *g_psyq_press_mdec_status = 0x80000000;
    *g_psyq_press_dma_in_chcr = 0;
    *g_psyq_press_dma_out_chcr = 0;
    result = 0;
    decoder_status = (u32)g_psyq_press_dma_out_chcr;
    decoder_control = g_psyq_press_mdec_status;
    decoder_status = *(volatile u32*)decoder_status;
    decoder_status = 0x60000000;
    *decoder_control = decoder_status;
    return result;
}
