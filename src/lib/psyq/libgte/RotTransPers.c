#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Transform and project `input` to the screen (RTPS): the screen position goes to `xy`, the depth-cue
 * factor to `p` and the error flags to `flag`; returns the depth / 4, an ordering-table index.
 * battle_effect_draw_billboard_sprite and battle_effect_draw_item_drop_sprite use it. */
s32 RotTransPers(SVECTOR* input, s32* xy, s32* p, s32* flag) {
    s32 depth;
    PSYQ_GTE_ROTPERSP(input, xy, p, flag, depth);
    return depth >> 2;
}
