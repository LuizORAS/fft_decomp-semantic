#include "psx/libetc.h"
#include "psx/libgpu.h"
s32 VSync(s32 mode) {
    u32 status = *g_psyq_etc_gpu_status;
    s32 elapsed = (*g_psyq_etc_timer1_count - g_psyq_etc_last_timer1_count) & 0xffff;
    if (mode < 0)
        return g_psyq_etc_vblank_count;
    if (mode == 1)
        return elapsed;
    v_wait(mode > 0 ? g_psyq_etc_last_vblank + (mode - 1) : g_psyq_etc_last_vblank, mode > 0 ? mode - 1 : 0);
    status = *g_psyq_etc_gpu_status;
    v_wait(g_psyq_etc_vblank_count + 1, 1);
    if ((status & PSYQ_GPU_STATUS_HEIGHT_480) && !((status ^ *g_psyq_etc_gpu_status) & PSYQ_GPU_STATUS_ODD_LINE)) {
        do { } while (!((status ^ *g_psyq_etc_gpu_status) & PSYQ_GPU_STATUS_ODD_LINE)); }
    g_psyq_etc_last_vblank = g_psyq_etc_vblank_count;
    g_psyq_etc_last_timer1_count = *g_psyq_etc_timer1_count;
    return elapsed;
}
