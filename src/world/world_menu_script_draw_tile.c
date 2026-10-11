#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Draws the rectangle described by a 6-byte command header (x, y, w, h)
 * followed by its payload, and returns the next command. */
u8* world_menu_script_draw_tile(u8* cmd) {
    world_menu_tile_command_t* command = (world_menu_tile_command_t*)cmd;
    RECT rect;
    s32 y;
    s16 step;

    if (g_world_menu_use_scroll_position == 0) {
        y = command->y;
    } else {
        step = g_world_menu_row_height;
        y = step * g_world_menu_scroll_row_offset + command->y + g_world_menu_scroll_pixel_offset;
        if (g_world_menu_scroll_pixel_offset < 0) {
            y -= step;
        }
    }
    rect.x = command->x;
    rect.y = y;
    rect.w = command->width;
    rect.h = command->height;
    world_gfx_enqueue_tile(&rect, &command->color, g_world_menu_semi_trans, g_world_menu_draw_priority);
    return (u8*)command + command->length;
}
