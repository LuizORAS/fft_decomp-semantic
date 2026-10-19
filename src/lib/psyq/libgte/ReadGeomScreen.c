#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Return the GTE's projection distance. GsSortSprite reads it. */
s32 ReadGeomScreen(void) {
    s32 screen;
    PSYQ_GTE_CFC2(PSYQ_GTE_CTRL_H, screen);
    return screen;
}
