/* LIBGPU 8002400c-8002418c. */
#include "psx/libgpu.h"
void DumpDrawEnv(DRAWENV* env) {
    g_psyq_gpu_printf(g_psyq_gpu_draw_clip_format, env->clip.x, env->clip.y, env->clip.w, env->clip.h);
    g_psyq_gpu_printf(g_psyq_gpu_draw_offset_format, env->ofs[0], env->ofs[1]);
    g_psyq_gpu_printf(g_psyq_gpu_texture_window_format, env->tw.x, env->tw.y, env->tw.w, env->tw.h);
    g_psyq_gpu_printf(g_psyq_gpu_dithering_format, env->dtd);
    g_psyq_gpu_printf(g_psyq_gpu_draw_display_enable_format, env->dfe);
    if (GetGraphType() == 1 || GetGraphType() == 2) {
        g_psyq_gpu_printf(g_psyq_gpu_tpage_format, (env->tpage >> 9) & 3, (env->tpage >> 7) & 3,
            (env->tpage << 6) & 0x7c0, (env->tpage << 3) & 0x300);
    } else {
        g_psyq_gpu_printf(g_psyq_gpu_tpage_format, (env->tpage >> 7) & 3, (env->tpage >> 5) & 3,
            (env->tpage << 6) & 0x7c0, ((env->tpage << 4) & 0x100) + ((env->tpage >> 2) & 0x200));
    }
}
