#include "fft/menu_script.h"

/* Opcode 9 (MENU_SCRIPT_OP_FIXED_CLUT): the CLUT at x = byte 3 * 16, y = byte 4 + byte 2 * 256
 * becomes the current one (g_menu_gfx_clut_id). Returns the next command. */
menu_instruction_t* menu_script_set_fixed_clut(menu_instruction_t* data) {
    g_menu_gfx_clut_id
        = GetClut(data->fixed_clut.x_div_16 << 4, data->fixed_clut.y_low | (data->fixed_clut.y_high << 8));
    return menu_next_instruction(data);
}
