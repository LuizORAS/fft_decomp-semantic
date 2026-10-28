#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Multiply `left`'s rotation by `right`'s into `output`, whose translation is untouched; the GTE's rotation
 * matrix is used for the product. battle_map_draw_mesh_and_weather uses it. */
MATRIX* MulMatrix0(MATRIX* left, MATRIX* right, MATRIX* output) {
    /* The destination and exposed retail result remain in a2 and v0. */
    register MATRIX* destination __asm__("$6") = output;
    register MATRIX* result __asm__("$2");
    PSYQ_GTE_LOAD_MATRIX(left, PSYQ_GTE_CTRL_R11_R12, PSYQ_GTE_CTRL_R13_R21, PSYQ_GTE_CTRL_R22_R23,
        PSYQ_GTE_CTRL_R31_R32, PSYQ_GTE_CTRL_R33);
    PSYQ_GTE_MATRIX_COLUMNS(right);
    PSYQ_GTE_MATRIX_STORE(destination);
    result = output;
    __asm__ volatile("" : : "r"(result)); /* Keep the return copy before JR and its empty delay slot. */
    return result;
}
