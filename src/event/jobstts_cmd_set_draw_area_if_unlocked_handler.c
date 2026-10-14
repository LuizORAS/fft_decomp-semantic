#include "fft/event_jobstts.h"
#include "fft/menu.h"

/* Stream command: enqueue a draw-area rect (x, y, w, h from the command)
 * unless the zoom transition already emitted one. */
menu_instruction_t* jobstts_cmd_set_draw_area_if_unlocked_handler(menu_instruction_t* data) {
    RECT area;

    if (g_jobstts_gfx_otag_index_locked == 0) {
        area.x = data->draw_area.x;
        area.y = data->draw_area.y;
        area.w = data->draw_area.width;
        area.h = data->draw_area.height;
        jobstts_gfx_enqueue_draw_area(&area, g_menu_gfx_otag_index - 1);
    }
    return menu_next_instruction(data);
}
