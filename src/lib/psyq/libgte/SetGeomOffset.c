#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Set the GTE's screen offset, the screen position of the projection center, to (`x`, `y`). The display
 * set-ups of MAIN, BATTLE, the effects and LIBGS set it. */
void SetGeomOffset(int x, int y) {
    x <<= 16;
    y <<= 16;
    PSYQ_GTE_CTC2(PSYQ_GTE_CTRL_OFX, x);
    PSYQ_GTE_CTC2(PSYQ_GTE_CTRL_OFY, y);
}
