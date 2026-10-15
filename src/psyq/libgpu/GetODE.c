/* LIBGPU 8002536c-800253a0. */
#include "psx/libgpu.h"

/* Return the GPU status's top bit, which tells whether the odd or the even lines are being drawn in
 * interlaced mode (psx-spx). No code refers to it. */
u32 GetODE(void) {
    return g_psyq_gpu_dispatch->status() >> 31;
}
