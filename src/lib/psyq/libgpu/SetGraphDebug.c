/* LIBGPU 800244a4-80024510. */
#include "psx/libgpu.h"

/* Set LIBGPU's debug level (above 0 it prints, 2 traces every call) and return the old one.
 * main_boot_run_startup and main_boot_reset_game_state set 0. */
int SetGraphDebug(int level) {
    u8 old = g_psyq_gpu_environment.debug_level;
    u8 debug_level = level;
    g_psyq_gpu_environment.debug_level = debug_level;
    if (debug_level) {
        g_psyq_gpu_printf(g_psyq_gpu_graph_debug_format, g_psyq_gpu_environment.debug_level,
            g_psyq_gpu_environment.graph_type, g_psyq_gpu_environment.reversed);
    }
    return old;
}
