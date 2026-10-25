#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Fade three colors towards the far color by `depth` (DPCT) into `out0`-`out2`; the code byte comes from
 * `c2`. No code refers to it. */
void DpqColor3(CVECTOR* c0, CVECTOR* c1, CVECTOR* c2, s32 depth, CVECTOR* out0, CVECTOR* out1, CVECTOR* out2) {
    PSYQ_GTE_DPCT(c0, c1, c2, depth, out0, out1, out2);
}
