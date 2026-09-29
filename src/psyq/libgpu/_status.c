/* LIBGPU 80025b44-80025b5c. */
#include "psx/libgpu.h"

u32 _status(void) {
    return *g_psyq_gpu_gp1_port;
}
