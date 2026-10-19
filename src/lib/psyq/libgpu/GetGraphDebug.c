/* LIBGPU 800245cc-800245dc. */
#include "psx/libgpu.h"

/* Return LIBGPU's debug level, which the game leaves at 0. */
int GetGraphDebug(void) {
    return g_psyq_gpu_environment.debug_level;
}
