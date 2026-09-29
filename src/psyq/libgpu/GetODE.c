/* LIBGPU 8002536c-800253a0. */
#include "psx/libgpu.h"

u32 GetODE(void) {
    return g_psyq_gpu_dispatch->status() >> 31;
}
