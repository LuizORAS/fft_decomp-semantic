#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Load `matrix`'s rotation into the GTE's rotation matrix. About 140 functions (the camera, effects, units,
 * the map) load their rotation this way before transforming points. */
void SetRotMatrix(MATRIX* matrix) {
    PSYQ_GTE_LOAD_MATRIX(matrix, PSYQ_GTE_CTRL_R11_R12, PSYQ_GTE_CTRL_R13_R21, PSYQ_GTE_CTRL_R22_R23,
        PSYQ_GTE_CTRL_R31_R32, PSYQ_GTE_CTRL_R33);
}
