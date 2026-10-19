#include "psx/libetc.h"
#include "psx/libpress.h"
#include "psx/types.h"

/* Set the routine called when the MDEC input DMA (channel 0) ends. No code refers to it. */
s32 DecDCTinCallback(psyq_interrupt_callback_t callback) {
    /* Returns the previous callback address as a signed 32-bit value. */
    return (s32)DMACallback(PSYQ_DMA_MDEC_IN_CHANNEL, callback);
}
