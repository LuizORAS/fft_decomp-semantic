#include "fft/event_bunit.h"
#include "fft/menu.h"

/* Opcode 0x0f (MENU_SCRIPT_OP_DRAW_PRIORITY): byte 3 becomes the draw priority, the ordering-table
 * index of the primitives the following commands queue (g_menu_gfx_otag_index). Returns the next
 * command. */
menu_instruction_t* bunit_menu_script_set_draw_priority(menu_instruction_t* data) {
    g_menu_gfx_otag_index = data->draw_state.value;
    return menu_next_instruction(data);
}
