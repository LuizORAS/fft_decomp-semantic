#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Opcode 0x0f in JOBSTTS's display scripts: byte 3 becomes the draw priority (g_jobstts_gfx_otag_index)
 * unless a temporary draw area is active (g_jobstts_gfx_otag_index_locked, set by
 * jobstts_menu_script_draw_opening_window_frame while a window opens). Returns the next command. */
const u8* jobstts_menu_script_set_draw_priority(const u8* data) {
    if (g_jobstts_gfx_otag_index_locked == 0) {
        g_jobstts_gfx_otag_index = data[3];
    }
    return data + data[1];
}
