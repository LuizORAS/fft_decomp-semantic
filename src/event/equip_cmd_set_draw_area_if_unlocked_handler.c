#include "fft/event_equip.h"
#include "fft/menu.h"

/* Stream command: enqueue a draw-area rect (x, y, w, h from the command)
   unless the zoom transition already emitted one. */
menu_instruction_t* equip_cmd_set_draw_area_if_unlocked_handler(menu_instruction_t* cmd) {
    RECT rect;

    if (g_equip_gfx_zoom_draw_area_active == 0) {
        rect.x = cmd->draw_area.x;
        rect.y = cmd->draw_area.y;
        rect.w = cmd->draw_area.width;
        rect.h = cmd->draw_area.height;
        menu_gfx_enqueue_draw_area(&rect, g_menu_gfx_otag_index - 1);
    }
    return menu_next_instruction(cmd);
}
