/* LIBGPU 80025a88-80025b44. */
#include "psx/libgpu.h"

/* The display's x start in VRAM for `rect`, adjusted for the extended GPU types (halved for type 2, mirrored
 * when the display is reversed). PutDispEnv uses it. */
int get_dx(RECT* rect) {
    extern volatile psyq_gpu_environment_t g_psyq_gpu_environment; /* The driver reads the environment as volatile. */
    switch (g_psyq_gpu_environment.graph_type) {
    case 1:
        return g_psyq_gpu_environment.reversed ? 1024 - rect->x - rect->w : rect->x;
    case 2:
        return g_psyq_gpu_environment.reversed ? 1024 - rect->x - rect->w / 2 : rect->x / 2;
    }
    return rect->x;
}
