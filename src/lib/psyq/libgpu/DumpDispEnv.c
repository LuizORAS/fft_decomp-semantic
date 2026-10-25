/* LIBGPU 8002418c-80024238. */
#include "psx/libgpu.h"

/* Print display environment `env`: display and screen rectangles, interlace and 24-bit color. No code refers
 * to it. */
void DumpDispEnv(DISPENV* env) {
    g_psyq_gpu_printf(g_psyq_gpu_display_rectangle_format, env->disp.x, env->disp.y, env->disp.w, env->disp.h);
    g_psyq_gpu_printf(g_psyq_gpu_screen_rectangle_format, env->screen.x, env->screen.y, env->screen.w, env->screen.h);
    g_psyq_gpu_printf(g_psyq_gpu_interlace_format, env->isinter);
    g_psyq_gpu_printf(g_psyq_gpu_rgb24_format, env->isrgb24);
}
