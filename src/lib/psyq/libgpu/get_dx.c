/* LIBGPU 80025a88-80025b44. */
#include "psx/libgpu.h"

/* The display's x start in VRAM for `rect`, adjusted for the extended GPU types (halved for type 2, mirrored
 * when the display is reversed). PutDispEnv uses it. */
int get_dx(RECT* rect) {
    /* The volatile view preserves separate graph-type address construction. */
    volatile psyq_gpu_environment_t* environment = &g_psyq_gpu_environment;
    switch (environment->graph_type) {
    case 1:
        return environment->reversed ? 1024 - rect->x - rect->w : rect->x;
    case 2:
        return environment->reversed ? 1024 - rect->x - rect->w / 2 : rect->x / 2;
    }
    return rect->x;
}
