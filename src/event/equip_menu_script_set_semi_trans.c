#include "fft/event_equip.h"
#include "fft/menu.h"

/* Opcode 0x0e (MENU_SCRIPT_OP_SEMI_TRANS): byte 3 turns semi-transparency on or off for the
 * primitives the following commands queue (g_menu_gfx_semitransparency). Returns the next command. */
menu_instruction_t* equip_menu_script_set_semi_trans(menu_instruction_t* data) {
    g_menu_gfx_semitransparency = data->draw_state.value;
    return menu_next_instruction(data);
}
