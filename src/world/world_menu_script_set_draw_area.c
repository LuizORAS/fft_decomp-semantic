#include "fft/world.h"

/* Apply a four-byte draw area from the menu script and advance the command. */
u8* world_menu_script_set_draw_area(u8* script) {
    world_menu_draw_area_command_t* command = (world_menu_draw_area_command_t*)script;
    RECT rect;

    if (g_world_menu_temporary_draw_area_active == 0) {
        rect.x = command->x;
        rect.y = command->y;
        rect.w = command->width;
        rect.h = command->height;
        world_gfx_add_draw_area_primitive(&rect, g_world_menu_draw_priority - 1);
    }
    return (u8*)command + command->length;
}
