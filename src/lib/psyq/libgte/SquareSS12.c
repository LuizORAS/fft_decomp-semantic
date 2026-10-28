#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Same as SquareSL12 into a 16-bit `output`. No code refers to it. */
SVECTOR* SquareSS12(SVECTOR* input, SVECTOR* output) {
    PSYQ_GTE_SVECTOR_SQUARE(input, output, PSYQ_GTE_CMD_SQUARE_12);
    return output;
}
