#include "psx/libetc.h"

/* Set the callback for DMA channel `channel`'s completions and return the previous one, enabling or
 * disabling the channel's interrupt (and the master enable) in the DMA interrupt register. DMACallback reaches
 * it through the dispatch table. */
psyq_interrupt_callback_t setIntrDMA(s32 channel, psyq_interrupt_callback_t callback) {
    psyq_interrupt_callback_t previous = g_psyq_etc_dma_callbacks[channel];
    if (callback != previous) {
        if (callback) {
            g_psyq_etc_dma_callbacks[channel] = callback;
            *g_psyq_etc_dma_dicr = (*g_psyq_etc_dma_dicr & PSYQ_DMA_IRQ_CONTROL_MASK) | PSYQ_DMA_IRQ_MASTER_ENABLE
                | (1 << (channel + PSYQ_DMA_IRQ_CHANNEL_ENABLE_SHIFT));
        } else {
            g_psyq_etc_dma_callbacks[channel] = 0;
            *g_psyq_etc_dma_dicr = ((*g_psyq_etc_dma_dicr & PSYQ_DMA_IRQ_CONTROL_MASK) | PSYQ_DMA_IRQ_MASTER_ENABLE)
                & ~(1 << (channel + PSYQ_DMA_IRQ_CHANNEL_ENABLE_SHIFT));
        }
    }
    return previous;
}
