#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Stream command closing a zoomed window: when the zoom transition queued a
 * cropped draw area, queue the full draw area again one ordering-table entry
 * above the window's contents, move on to the next entry and clear the
 * lock. */
u8* jobstts_menu_script_clear_temporary_draw_area(u8* data) {
    s16 otag_index;
    if (g_jobstts_gfx_otag_index_locked != 0) {
        otag_index = g_jobstts_gfx_otag_index + 1;
        g_jobstts_gfx_draw_area.y = 0;
        g_jobstts_gfx_otag_index = otag_index;
        jobstts_gfx_enqueue_draw_area(&g_jobstts_gfx_draw_area, otag_index);
        g_jobstts_gfx_otag_index_locked = 0;
        g_jobstts_gfx_otag_index = g_jobstts_gfx_otag_index + 1;
    }
    return data + data[1];
}
