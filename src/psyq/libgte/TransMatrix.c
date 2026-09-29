#include "psx/libgte.h"

/* main 0x8001cf48–0x8001cf6c: copy the translation vector, return matrix in v0. */
void TransMatrix(void* matrix_address, void* vector_address) {
    MATRIX* matrix = matrix_address;
    VECTOR* vector = vector_address;
    /* The three translation words retain the handwritten t0-t2 store order. */
    register s32 x __asm__("$8") = vector->vx; /* The library uses fixed scratch registers. */
    register s32 y __asm__("$9") = vector->vy;
    register s32 z __asm__("$10") = vector->vz;
    MATRIX* result;
    __asm__ volatile("" : : "r"(x), "r"(y), "r"(z));
    matrix->t[0] = x;
    matrix->t[1] = y;
    matrix->t[2] = z;
    __asm__ volatile("" : : : "memory");      /* Retail completes all stores before the return copy. */
    __asm__("" : "=r"(result) : "0"(matrix)); /* Keep the return copy after the stores. */
    __asm__ volatile("" : : "r"(result));
}
