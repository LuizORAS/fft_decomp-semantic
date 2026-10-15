#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Same as SquareSL0 into a 16-bit `output`. No code refers to it. */
SVECTOR* SquareSS0(SVECTOR* input, SVECTOR* output) {
    SVECTOR* destination = output;
    PSYQ_GTE_SVECTOR_SQUARE(input, destination, PSYQ_GTE_CMD_SQUARE_0);
    return output;
}
