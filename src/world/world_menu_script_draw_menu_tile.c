#include "fft/world.h"
#include "psx/types.h"

/* Draw the command's two endpoints with its RGB color, then advance. */
u8* world_menu_script_draw_menu_tile(u8* script) {
    world_menu_line_command_t* command = (world_menu_line_command_t*)script;
    RECT rect;

    rect.x = command->x0;
    rect.y = command->y0;
    rect.w = command->x1;
    rect.h = command->y1;
    world_gfx_enqueue_line_f2(&rect, &command->color, g_world_menu_semi_trans, g_world_menu_draw_priority);
    return (u8*)command + command->length;
}
