#include "fft/event_bunit.h"
#include "psx/types.h"

/* Stream command closing a zoomed window: when the zoom transition queued a
 * cropped draw area, queue the full draw area again at the next
 * ordering-table entry and clear the lock. Unlike
 * jobstts_cmd_release_otag_lock_handler, BUNIT does not advance the
 * ordering-table index here. */
menu_instruction_t* bunit_cmd_release_otag_lock_handler(menu_instruction_t* data) {
    if (g_bunit_gfx_otag_index_locked != 0) {
        g_bunit_gfx_draw_area.y = 0;
        bunit_gfx_enqueue_draw_area(&g_bunit_gfx_draw_area, g_bunit_gfx_otag_index + 1);
        g_bunit_gfx_otag_index_locked = 0;
    }
    return menu_next_instruction(data);
}
