#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Read the GTE's light direction matrix into `output`, with the background colour in its translation. No
 * code refers to it. */
void ReadLightMatrix(MATRIX* output) {
    PSYQ_GTE_READ_MATRIX(output, PSYQ_GTE_CTRL_L11_L12, PSYQ_GTE_CTRL_L13_L21, PSYQ_GTE_CTRL_L22_L23,
        PSYQ_GTE_CTRL_L31_L32, PSYQ_GTE_CTRL_L33, PSYQ_GTE_CTRL_RBK, PSYQ_GTE_CTRL_GBK, PSYQ_GTE_CTRL_BBK);
}
