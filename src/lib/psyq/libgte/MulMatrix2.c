#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Multiply `left`'s rotation by `right`'s into `right` (right = left x right), keeping `right`'s
 * translation; the GTE's rotation matrix is used for the product. battle_map_draw_mesh_and_weather uses it. */
MATRIX* MulMatrix2(MATRIX* left, MATRIX* right) {
    register MATRIX* right_operand __asm__("$5") = right;
    register MATRIX* result __asm__("$2");
    PSYQ_GTE_LOAD_MATRIX(left, PSYQ_GTE_CTRL_R11_R12, PSYQ_GTE_CTRL_R13_R21, PSYQ_GTE_CTRL_R22_R23,
        PSYQ_GTE_CTRL_R31_R32, PSYQ_GTE_CTRL_R33);
    PSYQ_GTE_MATRIX_COLUMNS(right_operand);
    PSYQ_GTE_MATRIX_STORE(right_operand);
    result = right;
    __asm__ volatile("" : : "r"(result)); /* Keep the return copy before JR and its empty delay slot. */
    return result;
}
