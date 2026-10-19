#include "psx/libetc.h"
#include "psx/libpress.h"
#include "psx/types.h"

/* Set the routine called when the MDEC output DMA (channel 1) ends. open_movie_init_stream installs the
 * player's slice handler and open_movie_stop_stream removes it. */
s32 DecDCToutCallback(psyq_interrupt_callback_t callback) {
    /* Returns the previous callback address as a signed 32-bit value. */
    return (s32)DMACallback(PSYQ_DMA_MDEC_OUT_CHANNEL, callback);
}
