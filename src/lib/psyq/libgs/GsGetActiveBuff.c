#include "psx/libgs.h"
#include "psx/types.h"

/* Return the index of the buffer being drawn, 0 or 1. WLDCORE picks its per-buffer ordering table and
 * packet area with it. */
s32 GsGetActiveBuff(void) {
    return PSDIDX;
}
