#include "fft/event_bunit.h"
#include "psx/types.h"

/* Opcode 0x0f (MENU_SCRIPT_OP_DRAW_PRIORITY): byte 3 becomes the draw priority, the ordering-table
 * index of the primitives the following commands queue (g_bunit_gfx_otag_index). Returns the next
 * command. */
u8* bunit_menu_script_set_draw_priority(u8* data) {
    g_bunit_gfx_otag_index = data[3];
    return data + data[1];
}
