#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Same as RotTrans with 16-bit (saturated) components in `output`; the third is stored as a full word, over
 * the vector's padding. wldcore_map_draw_path_animation and wldcore_map_draw_visible_routes use it. */
void RotTransSV(SVECTOR* input, SVECTOR* output, s32* flag) {
    s32 result;
    PSYQ_GTE_ROTTRANSSV(input, output, result);
    *flag = result;
}
