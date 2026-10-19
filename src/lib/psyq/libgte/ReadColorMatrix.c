#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Read the GTE's light colour matrix into `output`, with the far colour in its translation. No code refers
 * to it. */
void ReadColorMatrix(MATRIX* output) {
    PSYQ_GTE_READ_MATRIX(output, PSYQ_GTE_CTRL_LR1_LR2, PSYQ_GTE_CTRL_LR3_LG1, PSYQ_GTE_CTRL_LG2_LG3,
        PSYQ_GTE_CTRL_LB1_LB2, PSYQ_GTE_CTRL_LB3, PSYQ_GTE_CTRL_RFC, PSYQ_GTE_CTRL_GFC, PSYQ_GTE_CTRL_BFC);
}
