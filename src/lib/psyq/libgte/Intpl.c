#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Fade the vector `input`, taken as a colour, towards the far colour by `depth` (INTPL) into `output`. No
 * code refers to it. */
void Intpl(VECTOR* input, s32 depth, CVECTOR* output) {
    PSYQ_GTE_INTPLC(input, depth, output);
}
