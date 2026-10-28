#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Load `matrix`'s translation into the GTE's translation vector, as callers of SetRotMatrix do. */
void SetTransMatrix(MATRIX* matrix) {
    PSYQ_GTE_LOAD_TRANSLATION(matrix);
}
