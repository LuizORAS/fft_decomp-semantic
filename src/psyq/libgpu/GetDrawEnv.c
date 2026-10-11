/* LIBGPU 80024e4c-80024e84. */
#include "psx/libgpu.h"

DRAWENV* GetDrawEnv(DRAWENV* env) {
    psyq_api_memcpy(env, &g_psyq_gpu_draw_environment, sizeof(DRAWENV));
    return env;
}
