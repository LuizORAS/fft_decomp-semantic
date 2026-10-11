/* LIBGPU 800257c8-80025824. */
#include "psx/libgpu.h"

u32 get_mode(int dfe, int dtd, int tpage) {
    extern volatile psyq_gpu_environment_t
        g_psyq_gpu_environment; /* An unqualified RAM view folds the native ADDIU/LBU address sequence. */
    if (g_psyq_gpu_environment.graph_type == 1 || g_psyq_gpu_environment.graph_type == 2)
        return PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MODE) | (dtd ? 0x800 : 0) | (dfe ? 0x1000 : 0)
            | (tpage & 0x27ff);
    return PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MODE) | (dtd ? 0x200 : 0) | (dfe ? 0x400 : 0) | (tpage & 0x9ff);
}
