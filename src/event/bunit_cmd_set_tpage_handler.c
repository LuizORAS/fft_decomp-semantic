#include "fft/event_bunit.h"

/* Select the texture page and blend rate. */
menu_instruction_t* bunit_cmd_set_tpage_handler(menu_instruction_t* data) {
    g_bunit_gfx_texture_page = GetTPage(data->texture_page.depth, data->texture_page.y_blend >> 4,
        data->texture_page.x_div_16 << 4, data->texture_page.y_blend << 8);
    return menu_next_instruction(data);
}
