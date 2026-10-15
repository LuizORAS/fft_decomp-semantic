#include "psx/libgpu.h"

/* Return the GPU type _reset records: 0 for the standard GPU; 1 or 2 when the GPU keeps draw-mode bit 12;
 * 3 or 4 when it answers version 2; the higher of each pair when `mode` has bit 3, which also sets its VRAM
 * size. Types 1 and 2 take the other bit layout of the packet words; ResetGraph sizes VRAM from the type. */
int psyq_gpu_detect_type(int mode) {
    *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_GET_INFO) | PSYQ_GPU_INFO_VERSION;
    if ((*g_psyq_gpu_gp0_port & PSYQ_GPU_PARAMETER_MASK) != 2) {
        *g_psyq_gpu_gp0_port
            = (PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MODE) | 0x1000U) | (*g_psyq_gpu_gp1_port & 0x3fff);
        *g_psyq_gpu_gp0_port;
        if (!(*g_psyq_gpu_gp1_port & 0x1000))
            return 0;
        if (!(mode & 8))
            return 1;
        *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_VRAM_SIZE_V1) | 0x504U;
        return 2;
    }
    if (!(mode & 8))
        return 3;
    *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_VRAM_SIZE) | PSYQ_GPU_VRAM_2MB;
    return 4;
}
