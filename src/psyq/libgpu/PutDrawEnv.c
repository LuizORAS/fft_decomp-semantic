/* LIBGPU 80024cac-80024d70. */
#include "psx/libgpu.h"
DRAWENV* PutDrawEnv(DRAWENV* env) {
    DR_ENV* packet;
    if (g_psyq_gpu_environment.debug_level >= 2)
        g_psyq_gpu_printf(g_psyq_gpu_put_draw_env_format, env);
    packet = &env->dr_env;
    SetDrawEnv(packet, env);
    ((P_TAG*)packet)->addr = PSYQ_GPU_DMA_END;
    g_psyq_gpu_dispatch->enqueue_four(g_psyq_gpu_dispatch->ordering_table, packet, sizeof(*packet), 0);
    psyq_api_memcpy(&g_psyq_gpu_environment.draw, env, sizeof(*env));
    return env;
}
