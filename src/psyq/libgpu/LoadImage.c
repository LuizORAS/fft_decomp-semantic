/* LIBGPU 800248fc-80024960. */
#include "psx/libetc.h"
#include "psx/libgpu.h"

s32 LoadImage(RECT* rect, u32* pixels) {
    checkRECT(g_psyq_gpu_load_image_name, rect);
    return g_psyq_gpu_dispatch->enqueue_four(g_psyq_gpu_dispatch->load, rect, 8, (u32)pixels);
}
