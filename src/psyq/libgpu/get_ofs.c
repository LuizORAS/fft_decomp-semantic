/* LIBGPU 800259bc-80025a04. */
#include "psx/libgpu.h"

u32 get_ofs(s16 x, s16 y) {
    extern volatile psyq_gpu_environment_t
        g_psyq_gpu_environment; /* An unqualified RAM view folds the native ADDIU/LBU address sequence. */
    if (g_psyq_gpu_environment.graph_type == 1 || g_psyq_gpu_environment.graph_type == 2)
        return PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_OFFSET) | ((y & 0xfff) << 12) | (x & 0xfff);
    return PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_OFFSET) | ((y & 0x7ff) << 11) | (x & 0x7ff);
}
