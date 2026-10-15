/* LIBGPU 800244a4-80024510. */
#include "psx/libetc.h"
#include "psx/libgpu.h"

/* Set LIBGPU's debug level (above 0 it prints, 2 traces every call) and return the old one.
 * main_boot_run_startup and main_boot_reset_game_state set 0. */
int SetGraphDebug(int level) {
    u8* debug = &g_psyq_gpu_debug_level;
    int old = *debug;
    *debug = level;
    if ((u8)level) {
        g_psyq_gpu_printf(g_psyq_gpu_graph_debug_format, *debug, g_psyq_gpu_graph_type, g_psyq_gpu_graph_reverse);
    }
    return old;
}
