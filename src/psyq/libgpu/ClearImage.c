/* LIBGPU 80024868-800248fc. */
#include "psx/libetc.h"
#include "psx/libgpu.h"

void ClearImage(RECT* rect, u8 r, u8 g, u8 b) {
    checkRECT(g_psyq_gpu_clear_image_name, rect);
    g_psyq_gpu_dispatch->enqueue_four(g_psyq_gpu_dispatch->clear, rect, 8, (b << 16) | (g << 8) | r);
}
