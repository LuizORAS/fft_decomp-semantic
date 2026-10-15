/* LIBGPU 80024cac-80024d70. */
#include "psx/libgpu.h"

/* Make `env` the draw environment: build its packet (SetDrawEnv), send it through the queue and remember it.
 * Returns `env`. */
DRAWENV* PutDrawEnv(DRAWENV* env) {
    u8* debug = &g_psyq_gpu_environment.debug_level;
    DR_ENV* packet;
    if (*debug >= 2)
        g_psyq_gpu_printf(g_psyq_gpu_put_draw_env_format, env);
    packet = &env->dr_env;
    SetDrawEnv(packet, env);
    ((P_TAG*)packet)->addr = PSYQ_GPU_DMA_END;
    g_psyq_gpu_dispatch->enqueue_four(g_psyq_gpu_dispatch->ordering_table, packet, 64, 0);
    psyq_api_memcpy(&g_psyq_gpu_environment.draw, env, 92);
    return env;
}
