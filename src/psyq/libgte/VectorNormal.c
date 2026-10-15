#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Scale the 32-bit vector `input` to length 4096 (ONE) into `output` (psyq_gte_normalize_register_vector).
 * About 35 functions (unit steps, the camera, effects) take directions this way. */
void VectorNormal(VECTOR* input, VECTOR* output) {
    VECTOR* source = input;
    VECTOR* destination = output;
    /* The private worker fixes t0-t2/v0 and saves RA in a3. */
    register s32 x __asm__("$8");
    register s32 y __asm__("$9");
    register s32 z __asm__("$10");
    s32 sum;
    register u32 saved_ra __asm__("$7");
    x = source->vx;
    y = source->vy;
    z = source->vz;
    PSYQ_GTE_SAVE_RA(saved_ra);
    PSYQ_GTE_NORMALIZE_CALL(psyq_gte_normalize_register_vector, x, y, z, sum);
    PSYQ_GTE_RESTORE_RA(saved_ra);
    destination->vx = x;
    destination->vy = y;
    destination->vz = z;
}
