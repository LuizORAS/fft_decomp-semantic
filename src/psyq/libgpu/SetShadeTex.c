/* LIBGPU 80023c90-80023cb8. */
#include "psx/libgpu.h"

/* Turn `primitive`'s texture shading off (`enabled` set: the raw-texture bit) or on. About 70 functions use
 * it. */
void SetShadeTex(void* primitive, int enabled) {
    if (enabled) {
        ((P_TAG*)primitive)->code |= PSYQ_GPU_CODE_RAW_TEXTURE;
    } else {
        ((P_TAG*)primitive)->code &= ~PSYQ_GPU_CODE_RAW_TEXTURE;
    }
}
