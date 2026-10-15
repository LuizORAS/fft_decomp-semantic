#include "psx/libpress.h"

/* Wait until the MDEC output DMA has finished (MDEC_out_sync). No code refers to it. */
void DecDCToutSync(void) {
    MDEC_out_sync();
}
