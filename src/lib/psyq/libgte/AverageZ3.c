#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Return three depths' sum times the factor InitGeom sets (AVSZ3, 341: the sum / 12). No code refers to it. */
s32 AverageZ3(s32 z0, s32 z1, s32 z2) {
    s32 result;
    PSYQ_GTE_AVSZ3(z0, z1, z2, result);
    return result;
}
