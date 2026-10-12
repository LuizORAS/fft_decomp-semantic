#include "psx/libpress.h"

/* The linked wrapper always blocks; the SDK mode argument is ignored. */
s32 DecDCToutSync(s32 unused_mode) {
    return MDEC_out_sync();
}
