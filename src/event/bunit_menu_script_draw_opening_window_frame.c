#include "fft/event_bunit.h"
#include "psx/types.h"

/* Stream command for a window frame that zooms open: during the first four
 * transition frames it first queues a draw area cropped about the window's
 * centre to the zoom table's percentage of its size (and, for the first
 * three, withholds the stream's input), then draws the frame through
 * bunit_menu_script_draw_window_frame. */
world_menu_window_command_t* bunit_menu_script_draw_opening_window_frame(world_menu_window_command_t* command) {
    RECT area;
    s32 frame;
    s32 scale;

    frame = g_bunit_gfx_transition_frame;
    if (frame < 4) {
        if (frame < 3) {
            g_bunit_menu_display_script_input = 0;
        }
        scale = g_bunit_gfx_window_zoom_percent[frame];
        area.w = command->width * scale / 100;
        area.h = command->height * scale / 100;
        area.x = command->x + (command->width >> 1) - (area.w >> 1);
        area.y = command->y + (command->height >> 1) - (area.h >> 1);
        bunit_gfx_enqueue_draw_area(&area, g_bunit_gfx_otag_index - 1);
        g_bunit_gfx_otag_index_locked = 1;
    } else {
        g_bunit_gfx_otag_index_locked = 0;
    }
    return bunit_menu_script_draw_window_frame(command);
}
