#include "fft/event_equip.h"
#include "psx/types.h"

/* Stream command for a window frame that zooms open: during the first four
 * transition frames it first queues a draw area cropped about the window's
 * centre to the zoom table's percentage of its size (and, for the first
 * three, withholds the stream's input), then draws the frame through
 * equip_cmd_draw_window_frame_handler. */
menu_instruction_t* equip_cmd_set_zoomed_draw_area_handler(menu_instruction_t* command) {
    RECT area;
    s32 frame;
    s32 scale;

    frame = g_equip_gfx_transition_frame;
    if (frame < 4) {
        if (frame < 3) {
            g_equip_cmd_stream_input = 0;
        }
        scale = g_equip_gfx_window_zoom_percent[frame];
        area.w = command->window.width * scale / 100;
        area.h = command->window.height * scale / 100;
        area.x = command->window.x + (command->window.width >> 1) - (area.w >> 1);
        area.y = command->window.y + (command->window.height >> 1) - (area.h >> 1);
        menu_gfx_enqueue_draw_area(&area, g_equip_gfx_sprite_ot_index - 1);
        g_equip_gfx_zoom_draw_area_active = 1;
    } else {
        g_equip_gfx_zoom_draw_area_active = 0;
    }
    return equip_cmd_draw_window_frame_handler(command);
}
