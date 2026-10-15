#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Transform `input` by the GTE's rotation and translation into the 32-bit `output` (translation plus
 * rotation x input / 4096, MVMVA) and return the GTE's error flags, also stored in `flag`. About 100 functions
 * (the camera, effects, units, the map) transform their points this way. */
long RotTrans(SVECTOR* input, VECTOR* output, long* flag) {
    long result;
    PSYQ_GTE_ROTTRANS(input, output, result);
    *flag = result;
    return result;
}
