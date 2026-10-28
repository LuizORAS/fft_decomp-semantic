/* LIBGPU 80024d70-80024e4c. */
#include "psx/libgpu.h"

/* Draw the packet chain at `ot` after setting the draw environment `env`, whose packet heads the chain, and
 * remember `env` as the current one. No code refers to it. */
void DrawOTagEnv(u32* ot, DRAWENV* env) {
    if (g_psyq_gpu_environment.debug_level >= 2)
        g_psyq_gpu_printf(g_psyq_gpu_draw_otag_env_format, ot, env);
    SetDrawEnv(&env->dr_env, env);
    setaddr(&env->dr_env, ot);
    g_psyq_gpu_dispatch->enqueue_four(g_psyq_gpu_dispatch->ordering_table, &env->dr_env, sizeof(env->dr_env), 0);
    psyq_api_memcpy(&g_psyq_gpu_environment.draw, env, sizeof(*env));
}
