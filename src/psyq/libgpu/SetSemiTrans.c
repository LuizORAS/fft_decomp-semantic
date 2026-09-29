/* LIBGPU 80023c68-80023c90. */
#include "psx/libgpu.h"

void SetSemiTrans(void* primitive, int enabled) {
    if (enabled) {
        ((P_TAG*)primitive)->code |= PSYQ_GPU_CODE_SEMI_TRANSPARENT;
    } else {
        ((P_TAG*)primitive)->code &= ~PSYQ_GPU_CODE_SEMI_TRANSPARENT;
    }
}
