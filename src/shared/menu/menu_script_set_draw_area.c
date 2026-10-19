#include "fft/event_bunit.h"
#include "fft/menu.h"
#include "fft/menu_script.h"

/* Opcode 0x14 (MENU_SCRIPT_OP_DRAW_AREA): unless a temporary draw area is active
 * (g_bunit_gfx_otag_index_locked), queue the draw area of bytes 2..5 (x, y, w, h) at the draw priority - 1.
 * Returns the next command. */
menu_instruction_t* menu_script_set_draw_area(menu_instruction_t* data) {
    RECT area;

    if (g_bunit_gfx_otag_index_locked == 0) {
        area.x = data->draw_area.x;
        area.y = data->draw_area.y;
        area.w = data->draw_area.width;
        area.h = data->draw_area.height;
        menu_gfx_enqueue_draw_area(&area, g_menu_gfx_otag_index - 1);
    }
    return menu_next_instruction(data);
}
