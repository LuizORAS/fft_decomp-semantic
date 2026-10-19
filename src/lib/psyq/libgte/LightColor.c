#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Multiply the light intensities `input` by the light colour matrix and add the background colour (MVMVA)
 * into `output`. No code refers to it. */
void LightColor(VECTOR* input, VECTOR* output) {
    PSYQ_GTE_IR_VECTOR(
        input, output, PSYQ_GTE_CMD_LIGHT_COLOR, PSYQ_GTE_DATA_IR1, PSYQ_GTE_DATA_IR2, PSYQ_GTE_DATA_IR3);
}
