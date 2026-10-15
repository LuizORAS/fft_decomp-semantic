#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Same as Square0 for a 16-bit `input`. No code refers to it. */
VECTOR* SquareSL0(SVECTOR* input, VECTOR* output) {
    VECTOR* destination = output;
    PSYQ_GTE_SVECTOR_SQUARE_LONG(input, destination, PSYQ_GTE_CMD_SQUARE_0);
    return output;
}
