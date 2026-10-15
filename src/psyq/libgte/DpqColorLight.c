#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Light `color` by the intensities `input` and fade it towards the far colour by `depth` (DCPL) into
 * `output`. No code refers to it. */
void DpqColorLight(VECTOR* input, CVECTOR* color, s32 depth, CVECTOR* output) {
    PSYQ_GTE_DCPL(input, color, depth, output);
}
