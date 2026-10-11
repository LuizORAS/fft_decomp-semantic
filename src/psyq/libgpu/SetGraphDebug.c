/* LIBGPU 800244a4-80024510. */
#include "psx/libetc.h"
#include "psx/libgpu.h"

int SetGraphDebug(int level) {
    int old = g_psyq_gpu_environment.debug_level;
    g_psyq_gpu_environment.debug_level = level;
    if ((u8)level) {
        g_psyq_gpu_printf(g_psyq_gpu_graph_debug_format, g_psyq_gpu_environment.debug_level,
            g_psyq_gpu_environment.graph_type, g_psyq_gpu_environment.reversed);
    }
    return old;
}
