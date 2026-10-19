#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Return four depths' sum times the factor InitGeom sets (AVSZ4, 256: the sum / 16). No code refers to it. */
s32 AverageZ4(s32 z0, s32 z1, s32 z2, s32 z3) {
    s32 result;
    PSYQ_GTE_AVSZ4(z0, z1, z2, z3, result);
    return result;
}
