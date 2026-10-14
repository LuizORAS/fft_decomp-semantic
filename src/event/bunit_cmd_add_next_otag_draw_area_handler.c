#include "fft/event_bunit.h"
#include "fft/menu.h"
#include "psx/types.h"

/* BUNIT.OUT 001cb20c - When the OT index is not locked, reset the shared draw
 * area's y to 0 and enqueue it at the next OT slot. Twin of
 * jobstts_cmd_add_next_otag_draw_area_handler. */
menu_instruction_t* bunit_cmd_add_next_otag_draw_area_handler(menu_instruction_t* data) {
    if (g_bunit_gfx_otag_index_locked == 0) {
        s32 otag_index = g_menu_gfx_otag_index;
        g_bunit_gfx_draw_area.y = 0;
        menu_gfx_enqueue_draw_area(&g_bunit_gfx_draw_area, otag_index + 1);
    }
    return menu_next_instruction(data);
}
