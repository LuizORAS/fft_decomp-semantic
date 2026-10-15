/* LIBGPU 800259bc-80025a04. */
#include "psx/libgpu.h"

/* The drawing offset command word for (`x`, `y`), in the standard or extended GPU's bit layout. */
u32 get_ofs(int x, int y) {
    extern volatile psyq_gpu_environment_t g_psyq_gpu_environment; /* The driver reads the environment as volatile. */
    if (g_psyq_gpu_environment.graph_type == 1 || g_psyq_gpu_environment.graph_type == 2)
        return PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_OFFSET) | ((y & 0xfff) << 12) | (x & 0xfff);
    return PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_OFFSET) | ((y & 0x7ff) << 11) | (x & 0x7ff);
}
