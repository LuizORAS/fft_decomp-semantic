#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Same as Square0 with each square divided by 4096 (SQR with the 12-bit shift). No code refers to it. */
VECTOR* Square12(VECTOR* input, VECTOR* output) {
    register VECTOR* destination __asm__("$5") = output; /* Retail uses a1 until the return delay slot. */
    PSYQ_GTE_IR_VECTOR(
        input, destination, PSYQ_GTE_CMD_SQUARE_12, PSYQ_GTE_DATA_MAC1, PSYQ_GTE_DATA_MAC2, PSYQ_GTE_DATA_MAC3);
    return output;
}
