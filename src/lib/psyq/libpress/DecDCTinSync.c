#include "psx/libpress.h"

/* Wait until the MDEC has taken its input (MDEC_in_sync); the mode argument is ignored. No code refers
 * to it. */
s32 DecDCTinSync(s32 unused_mode) {
    return MDEC_in_sync();
}
