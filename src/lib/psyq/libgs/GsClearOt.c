#include "psx/libgs.h"
#include "psx/types.h"

void GsClearOt(u16 x, u16 y, GsOT* ot) {
    ot->offset = x;
    ot->point = y;
    ot->tag = ot->org + (1 << ot->length) - 1;
    ClearOTagR((u32*)ot->org, 1 << ot->length);
}
