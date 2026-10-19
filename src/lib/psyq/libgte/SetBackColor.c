#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Set the GTE's background (ambient) colour to `r`, `g`, `b` (stored times 16). The battle map's light and
 * weather code sets it. */
void SetBackColor(s32 r, s32 g, s32 b) {
    r <<= 4;
    g <<= 4;
    b <<= 4;
    PSYQ_GTE_CTC2(PSYQ_GTE_CTRL_RBK, r);
    PSYQ_GTE_CTC2(PSYQ_GTE_CTRL_GBK, g);
    PSYQ_GTE_CTC2(PSYQ_GTE_CTRL_BBK, b);
}
