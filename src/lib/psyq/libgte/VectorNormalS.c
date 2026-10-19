#include "psx/cpu_return_address_abi.h"
#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Same as VectorNormal for a 32-bit `input` into a 16-bit `output`, through VectorNormalSS's tail; returns
 * the input's sum of squares. No code refers to it. */
long VectorNormalS(VECTOR* input, SVECTOR* output) {
    /* The continuation receives wide components in the private t0-t2 ABI. */
    register s32 x __asm__("$8");
    register s32 y __asm__("$9");
    register s32 z __asm__("$10");
    x = input->vx;
    y = input->vy;
    z = input->vz;
    PSYQ_GTE_NORMAL_SHORT_TAIL(VectorNormalSS, output, x, y, z);
}
PSYQ_GTE_NORMAL_SHORT_TAIL_END();
