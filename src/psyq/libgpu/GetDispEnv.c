/* LIBGPU 80025334-8002536c. */
#include "psx/libgpu.h"

DISPENV* GetDispEnv(DISPENV* env) {
    psyq_api_memcpy(env, &g_psyq_gpu_display_environment, 20);
    return env;
}
