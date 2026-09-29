/* LIBGPU 80026cdc-80026d10. */
#include "psx/libetc.h"
#include "psx/libgpu.h"
void set_alarm(void) {
    g_psyq_gpu_sync_deadline = VSync(-1) + 240;
    g_psyq_gpu_sync_poll_count = 0;
}
