#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Load `matrix` as the GTE's light colour matrix. The battle map's light and weather code sets it. */
void SetColorMatrix(MATRIX* matrix) {
    PSYQ_GTE_LOAD_MATRIX(matrix, PSYQ_GTE_CTRL_LR1_LR2, PSYQ_GTE_CTRL_LR3_LG1, PSYQ_GTE_CTRL_LG2_LG3,
        PSYQ_GTE_CTRL_LB1_LB2, PSYQ_GTE_CTRL_LB3);
}
