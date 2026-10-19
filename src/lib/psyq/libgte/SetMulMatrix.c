#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Load `left`'s rotation times `right`'s into the GTE's rotation matrix and return `left`. No code refers to
 * it. */
MATRIX* SetMulMatrix(MATRIX* left, MATRIX* right) {
    register MATRIX* left_operand __asm__("$4") = left; /* Retail directly uses argument registers. */
    register MATRIX* result __asm__("$2");
    PSYQ_GTE_LOAD_MATRIX(left_operand, PSYQ_GTE_CTRL_R11_R12, PSYQ_GTE_CTRL_R13_R21, PSYQ_GTE_CTRL_R22_R23,
        PSYQ_GTE_CTRL_R31_R32, PSYQ_GTE_CTRL_R33);
    PSYQ_GTE_MATRIX_COLUMNS(right);
    PSYQ_GTE_MATRIX_TO_CONTROL();
    result = left;
    __asm__ volatile("" : : "r"(result));
    return result;
}
