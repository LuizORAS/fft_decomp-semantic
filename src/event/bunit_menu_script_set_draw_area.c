#include "fft/event_bunit.h"
#include "psx/types.h"

/* Opcode 0x14 (MENU_SCRIPT_OP_DRAW_AREA): unless a temporary draw area is active
 * (g_bunit_gfx_otag_index_locked), queue the draw area of bytes 2..5 (x, y, w, h) at the draw priority - 1.
 * Returns the next command. */
u8* bunit_menu_script_set_draw_area(u8* data) {
    RECT area;

    if (g_bunit_gfx_otag_index_locked == 0) {
        area.x = data[2];
        area.y = data[3];
        area.w = data[4];
        area.h = data[5];
        bunit_gfx_enqueue_draw_area(&area, g_bunit_gfx_otag_index - 1);
    }
    return data + data[1];
}
