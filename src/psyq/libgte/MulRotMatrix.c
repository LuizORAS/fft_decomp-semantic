#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Multiply the GTE's rotation matrix by `matrix`'s rotation into `matrix`, and return it. No code refers to
 * it. */
MATRIX* MulRotMatrix(MATRIX* matrix) {
    register MATRIX* input __asm__("$4") = matrix; /* Retail directly uses a0 until the result copy. */
    register MATRIX* result __asm__("$2");
    PSYQ_GTE_MATRIX_COLUMNS_WORD(input);
    PSYQ_GTE_MATRIX_STORE(input);
    result = matrix;
    __asm__ volatile("" : : "r"(result));
    return result;
}
