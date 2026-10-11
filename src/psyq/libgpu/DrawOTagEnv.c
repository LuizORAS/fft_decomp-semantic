/* LIBGPU 80024d70-80024e4c. */
#include "psx/libgpu.h"
void DrawOTagEnv(u32* ot, DRAWENV* env) {
    DR_ENV* packet;
    if (g_psyq_gpu_environment.debug_level >= 2)
        g_psyq_gpu_printf(g_psyq_gpu_draw_otag_env_format, ot, env);
    packet = &env->dr_env;
    SetDrawEnv(packet, env);
    ((P_TAG*)packet)->addr = (u32)ot;
    g_psyq_gpu_dispatch->enqueue_four(g_psyq_gpu_dispatch->ordering_table, packet, sizeof(*packet), 0);
    psyq_api_memcpy(&g_psyq_gpu_environment.draw, env, sizeof(*env));
}
