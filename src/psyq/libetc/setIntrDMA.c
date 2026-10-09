#include "psx/libetc.h"

psyq_interrupt_callback_t setIntrDMA(s32 channel, psyq_interrupt_callback_t callback) {
    s32 selected_channel = channel;
    /* The mask and flags pins preserve the original control-word register lifetimes. */
    psyq_interrupt_callback_t selected_callback = callback;
    psyq_interrupt_callback_t previous = g_psyq_etc_dma_callbacks[selected_channel];
    register u32 mask __asm__("$2");
    if (selected_callback != previous) {
        /* Splitting the mask retains the original DMA-pointer load before the low-word OR. */
        mask = PSYQ_DMA_IRQ_CONTROL_MASK & ~0xffffU;
        if (selected_callback) {
            s32 shift;
            register u32 flags __asm__("$2");
            u32 control;
            volatile u32* dma_control = g_psyq_etc_dma_dicr;

            mask |= 0xffff;
            g_psyq_etc_dma_callbacks[selected_channel] = selected_callback;

            control = *dma_control;

            shift = selected_channel + PSYQ_DMA_IRQ_CHANNEL_ENABLE_SHIFT;
            control &= mask;
            flags = 1 << shift;
            flags |= PSYQ_DMA_IRQ_MASTER_ENABLE;

            control |= flags;
            *dma_control = control;
        } else {
            s32 shift;
            volatile u32* dma_control = g_psyq_etc_dma_dicr;
            u32 control;
            mask |= 0xffff;
            g_psyq_etc_dma_callbacks[selected_channel] = 0;
            control = *dma_control;

            shift = selected_channel + PSYQ_DMA_IRQ_CHANNEL_ENABLE_SHIFT;
            control &= mask;
            control |= PSYQ_DMA_IRQ_MASTER_ENABLE;
            *dma_control = control & ~(1 << shift);
        }
    }
    return previous;
}
