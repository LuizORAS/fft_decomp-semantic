/* LIBGPU 80024390-800244a4. */
#include "psx/libgpu.h"
int SetGraphReverse(int reversed) {
    register int value __asm__("$17") = reversed;
    u8* state = &g_psyq_gpu_graph_reverse;
    int old;
    register psyq_gpu_dispatch_t* dispatch __asm__("$2");
    u32 cached;
    register u32 bits __asm__("$2");
    old = *state;
    if (g_psyq_gpu_debug_level >= 2)
        g_psyq_gpu_printf(g_psyq_gpu_graph_reverse_format, value);
    dispatch = g_psyq_gpu_dispatch;
    *state = value;
    cached = dispatch->cached_control(PSYQ_GPU_GP1_DISPLAY_MODE);
    if (*state) {
        bits = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DISPLAY_MODE);
        bits |= PSYQ_GPU_DISPLAY_REVERSE;
    } else {
        bits = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DISPLAY_MODE);
    }
    cached |= bits;
    dispatch = g_psyq_gpu_dispatch;
    dispatch->control(cached);
    if (g_psyq_gpu_graph_type == 2) {
        u32 command = (PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_VRAM_SIZE_V1) | 0x504U);
        psyq_gpu_dispatch_t* mode_dispatch = g_psyq_gpu_dispatch;
        if (g_psyq_gpu_graph_reverse)
            command = (PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_VRAM_SIZE_V1) | 0x501U);
        mode_dispatch->control(command);
    }
    return old;
}
