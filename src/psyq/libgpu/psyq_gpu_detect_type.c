#include "psx/libgpu.h"

/* LIBGPU, retail 0x80026e7c-0x80026f58. Detects draw-mode and extended GPU support. */
int psyq_gpu_detect_type(int mode) {
    register int result __asm__("$2");
    volatile u32* data;
    *g_psyq_gpu_gp1_port = (PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_GET_INFO) | PSYQ_GPU_INFO_VERSION);
    data = g_psyq_gpu_gp0_port;
    if ((*data & PSYQ_GPU_PARAMETER_MASK) != 2) {
        *data = (PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MODE) | 0x1000U) | (*g_psyq_gpu_gp1_port & 0x3fff);
        (void)*g_psyq_gpu_gp0_port;
        if (!(*g_psyq_gpu_gp1_port & 0x1000)) {
            return 0;
        } else {
            if (!(mode & 8)) {
                return 1;
            } else {
                *g_psyq_gpu_gp1_port = (PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_VRAM_SIZE_V1) | 0x504U);
                return 2;
            }
        }
    }
    if (mode & 8) {
        *g_psyq_gpu_gp1_port = (PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_VRAM_SIZE) | PSYQ_GPU_VRAM_2MB);
        result = 4;
    } else {
        result = 3;
    }
    return result;
}
