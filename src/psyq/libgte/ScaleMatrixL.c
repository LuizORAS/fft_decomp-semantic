#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* main 0x8001c9e0–0x8001cb04: row scaling, low-word products and word stores.
 * The packed view preserves writes to the matrix alignment word. */
void ScaleMatrixL(MATRIX* matrix, VECTOR* scale) {
    /* Bindings preserve the packed-halfword multiply and store registers. */
    register psyq_packed_matrix_t* packed_matrix __asm__("$4") = (psyq_packed_matrix_t*)matrix;
    register u32 packed __asm__("$8");
    register s32 low __asm__("$9");
    register s32 high __asm__("$10");
    register u32 scale_x __asm__("$11");
    register u32 scale_y __asm__("$12");
    register u32 scale_z __asm__("$13");
    MATRIX* result;
    packed = packed_matrix->words[0];
    scale_x = scale->vx;
    low = packed & 0xffff;
    __asm__("" : "=r"(low) : "0"(low)); /* Keep the retail redundant low-half mask. */
    low = (low << 16) >> 16;
    PSYQ_GTE_MULTU_BEGIN(low, scale_x);
    high = (s32)packed >> 16;
    scale_y = scale->vy;
    scale_z = scale->vz;
    __asm__ volatile("" : : "r"(scale_y), "r"(scale_z) : "memory");
    packed = packed_matrix->words[1];
    __asm__ volatile("" : "=r"(packed_matrix) : "0"(packed_matrix), "r"(packed) : "memory");
    __asm__("" : "=r"(result) : "0"((MATRIX*)packed_matrix));
    __asm__ volatile("" : : "r"(result)); /* Retail exposes the result pointer before its stores. */
    PSYQ_GTE_READ_LO(low);
    low = (low >> 12) & 0xffff;
    PSYQ_GTE_MULTU_BEGIN(high, scale_x);
    PSYQ_GTE_READ_LO(high);
    high = (high >> 12) << 16;
    low |= high;
    packed_matrix->words[0] = low;
    low = packed & 0xffff;
    __asm__("" : "=r"(low) : "0"(low));
    low = (low << 16) >> 16;
    PSYQ_GTE_MULTU_BEGIN(low, scale_x);
    high = (s32)packed >> 16;
    packed = packed_matrix->words[2];
    PSYQ_GTE_READ_LO(low);
    low = (low >> 12) & 0xffff;
    PSYQ_GTE_MULTU_BEGIN(high, scale_y);
    PSYQ_GTE_READ_LO(high);
    high = (high >> 12) << 16;
    low |= high;
    packed_matrix->words[1] = low;
    low = packed & 0xffff;
    __asm__("" : "=r"(low) : "0"(low));
    low = (low << 16) >> 16;
    PSYQ_GTE_MULTU_BEGIN(low, scale_y);
    high = (s32)packed >> 16;
    packed = packed_matrix->words[3];
    PSYQ_GTE_READ_LO(low);
    low = (low >> 12) & 0xffff;
    PSYQ_GTE_MULTU_BEGIN(high, scale_y);
    PSYQ_GTE_READ_LO(high);
    high = (high >> 12) << 16;
    low |= high;
    packed_matrix->words[2] = low;
    low = packed & 0xffff;
    __asm__("" : "=r"(low) : "0"(low));
    low = (low << 16) >> 16;
    PSYQ_GTE_MULTU_BEGIN(low, scale_z);
    high = (s32)packed >> 16;
    packed = packed_matrix->words[4];
    PSYQ_GTE_READ_LO(low);
    low = (low >> 12) & 0xffff;
    PSYQ_GTE_MULTU_BEGIN(high, scale_z);
    PSYQ_GTE_READ_LO(high);
    high = (high >> 12) << 16;
    low |= high;
    packed_matrix->words[3] = low;
    low = packed & 0xffff;
    __asm__("" : "=r"(low) : "0"(low));
    low = (low << 16) >> 16;
    PSYQ_GTE_MULTU_BEGIN(low, scale_z);
    PSYQ_GTE_READ_LO(low);
    low >>= 12;
    packed_matrix->words[4] = low;
}
