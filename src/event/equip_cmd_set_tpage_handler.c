#include "fft/event_equip.h"

/* Stream command: set the texture page from the command (colour mode
 * data->texture_page.depth, blend rate data->texture_page.y_blend >> 4, x data->texture_page.x_div_16 * 16, y
 * data->texture_page.y_blend * 256) and return the next command. */
menu_instruction_t* equip_cmd_set_tpage_handler(menu_instruction_t* data) {
    g_equip_gfx_texture_page = GetTPage(data->texture_page.depth, data->texture_page.y_blend >> 4,
        data->texture_page.x_div_16 << 4, data->texture_page.y_blend << 8);
    return menu_next_instruction(data);
}
