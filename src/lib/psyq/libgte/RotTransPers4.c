#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Same as RotTransPers for four points (RTPT for the first three, then RTPS), with the error flags of both
 * passes combined; returns the fourth point's depth / 4. GsSortSprite projects a rotated sprite's corners this
 * way. */
s32 RotTransPers4(
    SVECTOR* v0, SVECTOR* v1, SVECTOR* v2, SVECTOR* v3, s32* xy0, s32* xy1, s32* xy2, s32* xy3, s32* p, s32* flag) {
    register s32 first_flag __asm__("$3"); /* Retail carries the RTPT flag in v1 across RTPS. */
    s32 depth;
    PSYQ_GTE_ROTPERSP4_FIRST(v0, v1, v2, xy0, xy1, xy2, first_flag);
    PSYQ_GTE_ROTPERSP4_LAST(v3, xy3, p, flag, first_flag, depth);
    return depth >> 2;
}
