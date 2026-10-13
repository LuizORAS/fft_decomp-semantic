#include "fft/event_bunit.h"
#include "psx/types.h"

/* Stream command closing a zoomed window: when the zoom transition queued a
 * cropped draw area, queue the full draw area again at the next
 * ordering-table entry and clear the lock. Unlike
 * jobstts_menu_script_clear_temporary_draw_area, BUNIT does not advance the
 * ordering-table index here. */
u8* bunit_menu_script_clear_temporary_draw_area(u8* data) {
    if (g_bunit_gfx_otag_index_locked != 0) {
        g_bunit_gfx_draw_area.y = 0;
        bunit_gfx_enqueue_draw_area(&g_bunit_gfx_draw_area, g_bunit_gfx_otag_index + 1);
        g_bunit_gfx_otag_index_locked = 0;
    }
    return data + data[1];
}
