#include "psx/libpress.h"

/* Wait until the MDEC output DMA has finished (MDEC_out_sync); the mode argument is ignored. No code
 * refers to it. */
s32 DecDCToutSync(s32 unused_mode) {
    return MDEC_out_sync();
}
