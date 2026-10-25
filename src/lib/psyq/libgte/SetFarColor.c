#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Set the GTE's far color, the color depth cueing fades towards, to `r`, `g`, `b` (stored times 16).
 * psyq_gs_init_gte sets black. */
void SetFarColor(s32 r, s32 g, s32 b) {
    r <<= 4;
    g <<= 4;
    b <<= 4;
    PSYQ_GTE_CTC2(PSYQ_GTE_CTRL_RFC, r);
    PSYQ_GTE_CTC2(PSYQ_GTE_CTRL_GFC, g);
    PSYQ_GTE_CTC2(PSYQ_GTE_CTRL_BFC, b);
}
