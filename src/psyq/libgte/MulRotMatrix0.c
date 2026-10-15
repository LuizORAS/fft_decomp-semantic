#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Multiply the GTE's rotation matrix by `matrix`'s rotation into `output`, and return `output`. No code
 * refers to it. */
MATRIX* MulRotMatrix0(MATRIX* matrix, MATRIX* output) {
    MATRIX* input = matrix;
    /* The destination and returned pointer retain the handwritten a1/v0 transport. */
    register MATRIX* destination __asm__("$5") = output;
    register MATRIX* result __asm__("$2");
    PSYQ_GTE_MATRIX_COLUMNS(input);
    PSYQ_GTE_MATRIX_STORE(destination);
    result = output;
    __asm__ volatile("" : : "r"(result));
    return result;
}
