/* LIBGPU 80022eec-80022f2c. */
#include "psx/libgpu.h"

/* Make font window `window` the default for FntPrint and route LIBGPU's diagnostic output to it.
 * main_gfx_load_efc_fnt and main_gfx_reset_display call it. */
void SetDumpFnt(int window) {
    if (window < 0 || g_psyq_gpu_font_window_count < window)
        return;
    g_psyq_gpu_default_font_window = window;
    g_psyq_gpu_printf = (int (*)(const char*, ...))FntPrint;
}
