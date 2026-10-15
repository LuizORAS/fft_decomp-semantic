#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Same as OuterProduct0 with each component divided by 4096. No code refers to it. */
void OuterProduct12(VECTOR* a, VECTOR* b, VECTOR* output) {
    PSYQ_GTE_OUTER_PRODUCT(a, b, output, PSYQ_GTE_CMD_OUTER_PRODUCT_12);
}
