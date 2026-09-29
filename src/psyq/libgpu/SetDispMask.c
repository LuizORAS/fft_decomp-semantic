/* LIBGPU 80024638-800246d4. */
#include "psx/libgpu.h"
void SetDispMask(int mask) {
    u8* debug = &g_psyq_gpu_environment.debug_level;
    u32 command;
    psyq_gpu_dispatch_t* dispatch;
    if (*debug >= 2)
        g_psyq_gpu_printf(g_psyq_gpu_display_mask_format, mask);
    if (!mask)
        memset2(&g_psyq_gpu_environment.display, -1, 20);
    dispatch = g_psyq_gpu_dispatch;
    command = mask ? PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DISPLAY_ENABLE)
                   : (PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DISPLAY_ENABLE) | PSYQ_GPU_DISPLAY_DISABLED);
    dispatch->control(command);
}
