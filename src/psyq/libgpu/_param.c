/* LIBGPU 80026424-80026454. */
#include "psx/libgpu.h"

u32 _param(int command) {
    *g_psyq_gpu_gp1_port = command | PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_GET_INFO);
    return *g_psyq_gpu_gp0_port & PSYQ_GPU_PARAMETER_MASK;
}
