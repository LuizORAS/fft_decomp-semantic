#include "psx/libgs.h"
#include "psx/types.h"

void GsClearOt(u16 x, u16 y, GsOT* ot) {
    ot->offset = x;
    ot->point = y;
    ot->tag = ot->org + (1 << ot->log2n) - 1;
    ClearOTagR(ot->org, 1 << ot->log2n);
}
