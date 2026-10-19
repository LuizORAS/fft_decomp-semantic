/* LIBGPU 8002634c-80026374. */
#include "psx/libgpu.h"

/* Write `command` to GP1 and keep it in the GP1 command cache under its command byte. */
void _ctl(u32 command) {
    *g_psyq_gpu_gp1_port = command;
    g_psyq_gpu_gp1_cache[command >> 24] = command;
}
