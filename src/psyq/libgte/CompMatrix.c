#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* main 0x8001c308–0x8001c468: affine composition with short translation input.
 * The translation sums retain retail's overflow traps. */
MATRIX* CompMatrix(MATRIX* left, MATRIX* right, MATRIX* output) {
    MATRIX* left_operand = left;
    MATRIX* right_operand = right;
    /* Bindings preserve the handwritten destination and translation registers. */
    register MATRIX* destination __asm__("$6") = output;
    register s32 x __asm__("$8");
    register s32 y __asm__("$9");
    register s32 z __asm__("$10");
    register s32 left_x __asm__("$11");
    register s32 left_y __asm__("$12");
    register s32 left_z __asm__("$13");
    MATRIX* result;
    PSYQ_GTE_LOAD_MATRIX(left_operand, PSYQ_GTE_CTRL_R11_R12, PSYQ_GTE_CTRL_R13_R21, PSYQ_GTE_CTRL_R22_R23,
        PSYQ_GTE_CTRL_R31_R32, PSYQ_GTE_CTRL_R33);
    PSYQ_GTE_MATRIX_COLUMNS(right_operand);
    PSYQ_GTE_COMPOSE_STORE(destination, right_operand);
    PSYQ_GTE_READ_MAC(x, y, z);
    left_x = left_operand->t[0];
    left_y = left_operand->t[1];
    left_z = left_operand->t[2];
    __asm__ volatile(""
        :
        : "r"(left_x), "r"(left_y), "r"(left_z)); /* Finish all input loads before the trapping sums. */
    PSYQ_CPU_TRAP_ADD(x, x, left_x);
    PSYQ_CPU_TRAP_ADD(y, y, left_y);
    PSYQ_CPU_TRAP_ADD(z, z, left_z);
    destination->t[0] = x;
    destination->t[1] = y;
    destination->t[2] = z;
    __asm__ volatile("" : "=r"(destination) : "0"(destination) : "memory");
    __asm__("" : "=r"(result) : "0"(destination));
    return result;
}
