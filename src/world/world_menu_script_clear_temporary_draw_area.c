#include "fft/world.h"

/* Remove the active temporary draw-area clip and advance the menu script. */
menu_instruction_t* world_menu_script_clear_temporary_draw_area(menu_instruction_t* instruction) {
    if (g_world_menu_temporary_draw_area_active != 0) {
        g_world_gfx_temporary_draw_area.y = 0;
        world_gfx_add_draw_area_primitive(&g_world_gfx_temporary_draw_area, g_world_menu_draw_priority + 1);
        g_world_menu_temporary_draw_area_active = 0;
    }
    return menu_next_instruction(instruction);
}
