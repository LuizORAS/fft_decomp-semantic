#include "psx/libgs.h"
#include "psx/types.h"

/* Empty the ordering table `ot`, with `x` as its offset and `y` as its point, and point its tag at its last
 * entry (ClearOTagR links the entries in reverse). WLDCORE clears its table each frame. */
void GsClearOt(u16 x, u16 y, GsOT* ot) {
    ot->offset = x;
    ot->point = y;
    ot->tag = (u32*)((s32)ot->org + (4 << ot->log2n) - 4);
    ClearOTagR(ot->org, 1 << ot->log2n);
}
