#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Multiply `left`'s rotation by `right`'s into `left` (left = left x right), keeping `left`'s translation;
 * the GTE's rotation matrix is used for the product and left holding `left`'s old rotation. The battle and
 * world map light threads use it. */
void MulMatrix(MATRIX* left, MATRIX* right) {
    register MATRIX* left_operand __asm__("$4") = left; /* Retail directly uses argument registers. */
    MATRIX* right_operand = right;
    register MATRIX* result __asm__("$2");
    PSYQ_GTE_LOAD_MATRIX(left_operand, PSYQ_GTE_CTRL_R11_R12, PSYQ_GTE_CTRL_R13_R21, PSYQ_GTE_CTRL_R22_R23,
        PSYQ_GTE_CTRL_R31_R32, PSYQ_GTE_CTRL_R33);
    PSYQ_GTE_MATRIX_COLUMNS(right_operand);
    PSYQ_GTE_MATRIX_STORE(left_operand);
    result = left;
    __asm__ volatile("" : : "r"(result)); /* Preserve the retail return register with the public void ABI. */
}
