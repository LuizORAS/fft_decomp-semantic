#include "fft/event_bunit.h"
#include "fft/menu.h"

/* Stream command closing a zoomed window: when the zoom transition queued a
 * cropped draw area, queue the full draw area again at the next
 * ordering-table entry and clear the lock. Unlike
 * jobstts_menu_script_clear_temporary_draw_area, BUNIT does not advance the
 * ordering-table index here. */
menu_instruction_t* bunit_menu_script_clear_temporary_draw_area(menu_instruction_t* data) {
    if (g_bunit_gfx_otag_index_locked != 0) {
        g_bunit_gfx_draw_area.y = 0;
        menu_gfx_enqueue_draw_area(&g_bunit_gfx_draw_area, g_menu_gfx_otag_index + 1);
        g_bunit_gfx_otag_index_locked = 0;
    }
    return menu_next_instruction(data);
}
