#include "fft/event_jobstts.h"

/* Stream command: when the ordering-table index is not locked, reset the
 * shared draw area's y to 0 and enqueue it at the next ordering-table
 * entry. */
const u8* jobstts_menu_script_reregister_temporary_draw_area(const u8* data) {
    if (g_jobstts_gfx_otag_index_locked == 0) {
        g_jobstts_gfx_draw_area.y = 0;
        jobstts_gfx_enqueue_draw_area(&g_jobstts_gfx_draw_area, g_jobstts_gfx_otag_index + 1);
    }
    return data + data[1];
}
