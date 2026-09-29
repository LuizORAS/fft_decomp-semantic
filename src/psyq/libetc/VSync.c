#include "psx/libetc.h"
#include "psx/libgpu.h"
s32 VSync(s32 mode) {
    u32 status = *g_psyq_etc_gpu_status;
    s32 elapsed = (*g_psyq_etc_timer1_count - g_psyq_etc_last_timer1_count) & 0xffff;
    /* An unpinned target inserts an extra mode copy before the first branch. */
    register s32 target __asm__("$2");
    s32 frames;
    if (mode < 0)
        return g_psyq_etc_vblank_count;
    if (mode == 1)
        return elapsed;
    if (mode > 0) {
        target = g_psyq_etc_last_vblank - 1; /* Keep subtraction before mode addition and the original branch delay. */
        __asm__("" : "=r"(target) : "0"(target));
        target += mode;
    } else
        target = g_psyq_etc_last_vblank;
    if (mode > 0)
        frames = mode - 1;
    else
        frames = 0;
    v_wait(target, frames);
    status = *g_psyq_etc_gpu_status;
    v_wait(g_psyq_etc_vblank_count + 1, 1);
    if (status & PSYQ_GPU_STATUS_HEIGHT_480) {
        volatile u32* gpu = g_psyq_etc_gpu_status;
        if ((s32)(status ^ *gpu) >= 0)
            do { } while (!((status ^ *gpu) & PSYQ_GPU_STATUS_ODD_LINE)); }
    g_psyq_etc_last_vblank = g_psyq_etc_vblank_count;
    g_psyq_etc_last_timer1_count = *g_psyq_etc_timer1_count;
    return elapsed;
}
