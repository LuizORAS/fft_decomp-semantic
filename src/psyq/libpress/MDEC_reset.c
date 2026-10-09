#include "psx/libc.h"
#include "psx/libpress.h"
#include "psx/types.h"

/* Drop the decoder back to a known state. Mode 0 also re-uploads the cached
 * quantisation and IDCT tables. */
void MDEC_reset(s32 mode) {
    switch (mode) {
    case 0:
        *g_psyq_press_mdec_status = 0x80000000;
        *g_psyq_press_dma_in_chcr = 0;
        *g_psyq_press_dma_out_chcr = 0;
        *g_psyq_press_mdec_status = 0x60000000;
        MDEC_in(g_psyq_press_quant_table_command, 0x20);
        MDEC_in(g_psyq_press_scale_table_command, 0x20);
        break;
    case 1:
        *g_psyq_press_mdec_status = 0x80000000;
        *g_psyq_press_dma_in_chcr = 0;
        *g_psyq_press_dma_out_chcr = 0;
        *g_psyq_press_dma_out_chcr;
        *g_psyq_press_mdec_status = 0x60000000;
        break;
    default:
        printf(g_psyq_press_reset_bad_option_format, mode);
        break;
    }
}
