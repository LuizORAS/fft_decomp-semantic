#include "psx/libetc.h"

/* Set the callback for the end of DMA channel `dma_channel`'s transfers and return the previous one
 * (setIntrDMA, through the dispatch table). The MDEC (channels 0 and 1), LIBGPU's queue (2), LIBCD (3) and
 * LIBSPU (4) install theirs with it. */
psyq_interrupt_callback_t DMACallback(s32 channel, psyq_interrupt_callback_t callback) {
    return g_psyq_etc_dispatch->dma_callback(channel, callback);
}
