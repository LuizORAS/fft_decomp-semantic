#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* main 0x8001c468–0x8001c574: rotation-only multiply; translation untouched. */
void MulMatrix0(MATRIX* left, void* right, MATRIX* output) {
    MATRIX* left_operand = left;
    void* right_operand = right;
    /* The destination and exposed retail result remain in a2 and v0. */
    register MATRIX* destination __asm__("$6") = output;
    register MATRIX* result __asm__("$2");
    PSYQ_GTE_LOAD_MATRIX(left_operand, PSYQ_GTE_CTRL_R11_R12, PSYQ_GTE_CTRL_R13_R21, PSYQ_GTE_CTRL_R22_R23,
        PSYQ_GTE_CTRL_R31_R32, PSYQ_GTE_CTRL_R33);
    PSYQ_GTE_MATRIX_COLUMNS(right_operand);
    PSYQ_GTE_MATRIX_STORE(destination);
    result = output;
    __asm__ volatile("" : : "r"(result)); /* Preserve the retail return register despite the public void ABI. */
}
