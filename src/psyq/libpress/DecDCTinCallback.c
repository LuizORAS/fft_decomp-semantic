#include "psx/libetc.h"
#include "psx/libpress.h"
#include "psx/types.h"

/* Set the routine called when the MDEC input DMA (channel 0) ends. No code refers to it. */
void DecDCTinCallback(void* callback) {
    DMACallback(0, callback);
}
