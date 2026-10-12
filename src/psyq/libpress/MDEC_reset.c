#include "psx/libpress.h"
#include "psx/types.h"

/* Drop the decoder back to a known state. Mode 0 also re-uploads the cached
 * quantisation and IDCT tables. */
void MDEC_reset(s32 mode) {
    switch (mode) {
    case 0:
        *g_psyq_press_mdec_status = PSYQ_MDEC_CONTROL_RESET;
        *g_psyq_press_dma_in_chcr = 0;
        *g_psyq_press_dma_out_chcr = 0;
        *g_psyq_press_mdec_status = PSYQ_MDEC_CONTROL_DMA_IN | PSYQ_MDEC_CONTROL_DMA_OUT;
        MDEC_in(&g_psyq_press_quant_table.command,
            (sizeof(g_psyq_press_quant_table.iq_y) + sizeof(g_psyq_press_quant_table.iq_c)) / sizeof(u32));
        MDEC_in(&g_psyq_press_scale_table.command, sizeof(g_psyq_press_scale_table.dct) / sizeof(u32));
        break;
    case 1:
        *g_psyq_press_mdec_status = PSYQ_MDEC_CONTROL_RESET;
        *g_psyq_press_dma_in_chcr = 0;
        *g_psyq_press_dma_out_chcr = 0;
        *g_psyq_press_dma_out_chcr;
        *g_psyq_press_mdec_status = PSYQ_MDEC_CONTROL_DMA_IN | PSYQ_MDEC_CONTROL_DMA_OUT;
        break;
    default:
        printf(g_psyq_press_reset_bad_option_format, mode);
        break;
    }
}
