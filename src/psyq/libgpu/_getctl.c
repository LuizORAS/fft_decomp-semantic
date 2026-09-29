/* LIBGPU 80026374-80026388. */
#include "psx/libgpu.h"

u32 _getctl(int command) {
    return g_psyq_gpu_gp1_cache[command];
}
