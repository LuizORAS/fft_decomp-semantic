#include "psx/libetc.h"

psyq_callback_setter_t startIntrDMA(void) {
    psyq_etc_clear_dma_callback_words((u32*)g_psyq_etc_dma_callbacks, PSYQ_ETC_CALLBACK_SLOTS);
    *g_psyq_etc_dma_dicr = 0;
    InterruptCallback(PSYQ_IRQ_DMA, trapIntrDMA);
    return setIntrDMA;
}
