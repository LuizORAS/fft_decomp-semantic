#include "fft/event_bunit.h"

/* Twin of jobstts_cmd_set_draw_area_if_unlocked_handler. */
menu_instruction_t* bunit_cmd_set_draw_area_if_unlocked_handler(menu_instruction_t* data) {
    RECT area;

    if (g_bunit_gfx_otag_index_locked == 0) {
        area.x = data->draw_area.x;
        area.y = data->draw_area.y;
        area.w = data->draw_area.width;
        area.h = data->draw_area.height;
        bunit_gfx_enqueue_draw_area(&area, g_bunit_gfx_otag_index - 1);
    }
    return menu_next_instruction(data);
}
