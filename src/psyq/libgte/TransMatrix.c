#include "psx/libgte.h"

/* Set the translation of the matrix at `matrix_address` to the vector at `vector_address`. The battle camera,
 * the battle map draw, GsSortSprite and WLDCORE's map projection use it. */
void TransMatrix(void* matrix_address, void* vector_address) {
    MATRIX* matrix = matrix_address;
    VECTOR* vector = vector_address;
    /* The three translation words retain the handwritten t0-t2 store order. */
    register s32 x __asm__("$8") = vector->vx; /* The library uses fixed scratch registers. */
    register s32 y __asm__("$9") = vector->vy;
    register s32 z __asm__("$10") = vector->vz;
    MATRIX* result;
    matrix->t[0] = x;
    matrix->t[1] = y;
    matrix->t[2] = z;
    __asm__ volatile("" : : : "memory");      /* Retail completes all stores before the return copy. */
    __asm__("" : "=r"(result) : "0"(matrix)); /* Keep the return copy after the stores. */
    __asm__ volatile("" : : "r"(result));
}
