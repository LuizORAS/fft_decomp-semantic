#include "fft/world.h"

/* Opcode 0x12: draw a window frame (world_menu_script_draw_window_frame) through the clip of the
 * window's opening animation. While the scale step is below 4 the clip is the window's rectangle scaled
 * by g_world_menu_window_scale_table about its centre (and below 3 the script input is cleared); from
 * step 4 no clip is set. */
void world_menu_script_draw_opening_window_frame(world_menu_window_command_t* command) {
    RECT clip;
    s32 scale;

    if (g_world_menu_window_scale_step < 4) {
        if (g_world_menu_window_scale_step < 3) {
            g_world_menu_display_script_input = 0;
        }
        scale = g_world_menu_window_scale_table[g_world_menu_window_scale_step];
        clip.w = command->width * scale / 100;
        clip.h = command->height * scale / 100;
        clip.x = command->x + (command->width >> 1) - (clip.w >> 1);
        clip.y = command->y + (command->height >> 1) - (clip.h >> 1);
        world_gfx_add_draw_area_primitive(&clip, g_world_menu_draw_priority - 1);
        g_world_menu_temporary_draw_area_active = 1;
    } else {
        g_world_menu_temporary_draw_area_active = 0;
    }
    world_menu_script_draw_window_frame(command);
}
