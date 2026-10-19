#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Same as Square12 for a 16-bit `input`. No code refers to it. */
VECTOR* SquareSL12(SVECTOR* input, VECTOR* output) {
    PSYQ_GTE_SVECTOR_SQUARE_LONG(input, output, PSYQ_GTE_CMD_SQUARE_12);
    return output;
}
