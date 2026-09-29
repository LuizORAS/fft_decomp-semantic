/* LIBETC 8001e614-8001e798; DMA completion dispatch and error diagnosis. */
#include "psx/libetc.h"

void trapIntrDMA(void) {
    u32 pending = (*g_psyq_etc_dma_dicr >> PSYQ_DMA_IRQ_FLAG_SHIFT) & PSYQ_DMA_IRQ_CHANNEL_MASK;
    s32 channel;
    u32 one;
    u32 control_mask;
    psyq_interrupt_callback_t* callbacks;
    psyq_interrupt_callback_t* callback;
    volatile u32* interrupt;
    if (pending) {
        one = 1;
        control_mask = PSYQ_DMA_IRQ_CONTROL_MASK;
        callbacks = g_psyq_etc_dma_callbacks;
        do {
            channel = 0;
            if (pending) {
                callback = callbacks;
                while (pending) {
                    if (channel >= PSYQ_DMA_CHANNEL_COUNT)
                        break;
                    if (pending & 1) {
                        *g_psyq_etc_dma_dicr &= (one << (channel + PSYQ_DMA_IRQ_FLAG_SHIFT)) | control_mask;
                        if (*callback)
                            (*callback)();
                    }
                    callback++;
                    pending >>= 1;
                    channel++;
                }
            }
            pending = (*g_psyq_etc_dma_dicr >> PSYQ_DMA_IRQ_FLAG_SHIFT) & PSYQ_DMA_IRQ_CHANNEL_MASK;
        } while (pending);
    }
    interrupt = g_psyq_etc_dma_dicr;
    if ((*interrupt & PSYQ_DMA_IRQ_FLAGS_MASK) == PSYQ_DMA_IRQ_MASTER_FLAG || (*interrupt & PSYQ_DMA_IRQ_BUS_ERROR)) {
        printf(g_psyq_etc_dma_error_format, *interrupt);
        for (channel = 0; channel < PSYQ_DMA_CHANNEL_COUNT; channel++) {
            printf(g_psyq_etc_dma_channel_address_format, channel, g_psyq_etc_dma_channels[channel].address);
        }
    }
}
