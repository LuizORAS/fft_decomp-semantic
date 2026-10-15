#include "psx/libetc.h"
#include "psx/libpress.h"
#include "psx/types.h"

/* Set the routine called when the MDEC output DMA (channel 1) ends. open_movie_init_stream installs the
 * player's slice handler and open_movie_stop_stream removes it. */
void DecDCToutCallback(void* callback) {
    DMACallback(1, callback);
}
