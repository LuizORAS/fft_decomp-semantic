#include "fft/menu_script.h"

/* Opcode 0x0c (MENU_SCRIPT_OP_TEXTURE_PAGE): compute the texture page from bytes 2..4 (GetTPage: colour
 * mode byte 4, blend rate byte 2 >> 4, x byte 3 * 16, y byte 2 * 256) and keep it for the following
 * quads (g_menu_gfx_texture_page). Returns the next command. */
menu_instruction_t* menu_script_set_texture_page(menu_instruction_t* data) {
    g_menu_gfx_texture_page = GetTPage(data->texture_page.depth, data->texture_page.y_blend >> 4,
        data->texture_page.x_div_16 << 4, data->texture_page.y_blend << 8);
    return menu_next_instruction(data);
}
