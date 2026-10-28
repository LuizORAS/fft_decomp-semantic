/* LIBGPU 80025334-8002536c. */
#include "psx/libgpu.h"

/* Copy the current display environment into `env` and return it. No code refers to it. */
DISPENV* GetDispEnv(DISPENV* env) {
    psyq_api_memcpy(env, &g_psyq_gpu_environment.display, sizeof(DISPENV));
    return env;
}
