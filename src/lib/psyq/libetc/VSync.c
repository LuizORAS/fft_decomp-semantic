#include "psx/libetc.h"
#include "psx/libgpu.h"

/* Wait for the vertical blank and return the horizontal blanks (root counter 1) counted from the previous
 * VSync to this call. Mode 0 waits for the next VBlank; mode n > 1 waits until n VBlanks have passed since the
 * previous VSync, and at least one; in 480-line mode it also waits for the field to change. Mode 1 returns the
 * count without waiting, and a negative mode the VBlanks counted since startIntrVSync. Each wait gives up
 * after a timeout (v_wait). The game's frame loops and waits pace themselves with it. */
s32 VSync(s32 mode) {
    u32 status = *g_psyq_etc_gpu_status;
    s32 elapsed = (*g_psyq_etc_timer1_count - Hcount) & 0xffff;
    if (mode < 0)
        return Vcount;
    if (mode == 1)
        return elapsed;
    v_wait(mode > 0 ? g_psyq_etc_last_vblank + (mode - 1) : g_psyq_etc_last_vblank, mode > 0 ? mode - 1 : 0);
    status = *g_psyq_etc_gpu_status;
    v_wait(Vcount + 1, 1);
    if ((status & PSYQ_GPU_STATUS_HEIGHT_480) && !((status ^ *g_psyq_etc_gpu_status) & PSYQ_GPU_STATUS_ODD_LINE)) {
        do { } while (!((status ^ *g_psyq_etc_gpu_status) & PSYQ_GPU_STATUS_ODD_LINE)); }
    g_psyq_etc_last_vblank = Vcount;
    Hcount = *g_psyq_etc_timer1_count;
    return elapsed;
}
