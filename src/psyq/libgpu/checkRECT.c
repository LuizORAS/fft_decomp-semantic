/* LIBGPU 80024740-80024868: debug-only rectangle diagnosis. */
#include "psx/libgpu.h"

void checkRECT(const char* name, RECT* rect) {
    switch (g_psyq_gpu_debug_level) {
    case 1:
        if (rect->w > (s16)g_psyq_gpu_vram_width || rect->w + rect->x > (s16)g_psyq_gpu_vram_width
            || rect->y > (s16)g_psyq_gpu_vram_height || rect->y + rect->h > (s16)g_psyq_gpu_vram_height || rect->w <= 0
            || rect->x < 0 || rect->y < 0 || rect->h <= 0) {
            g_psyq_gpu_printf(g_psyq_gpu_bad_rectangle_format, name);
            g_psyq_gpu_printf(g_psyq_gpu_rectangle_format, rect->x, rect->y, rect->w, rect->h);
        }
        break;
    case 2:
        g_psyq_gpu_printf(g_psyq_gpu_rectangle_operation_format, name);
        g_psyq_gpu_printf(g_psyq_gpu_rectangle_format, rect->x, rect->y, rect->w, rect->h);
        break;
    }
}
