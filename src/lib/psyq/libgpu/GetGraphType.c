/* LIBGPU 800245bc-800245cc. */
#include "psx/libgpu.h"

/* Return the GPU type ResetGraph detected (0 on the standard GPU). */
int GetGraphType(void) {
    return g_psyq_gpu_environment.graph_type;
}
