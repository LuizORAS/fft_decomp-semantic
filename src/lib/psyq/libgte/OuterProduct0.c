#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* The cross product of `a` and `b` (OP, no shift) into `output`, using the GTE's rotation diagonal as an
 * operand and restoring it. No code refers to it. */
void OuterProduct0(VECTOR* a, VECTOR* b, VECTOR* output) {
    PSYQ_GTE_OUTER_PRODUCT(a, b, output, PSYQ_GTE_CMD_OUTER_PRODUCT_0);
}
