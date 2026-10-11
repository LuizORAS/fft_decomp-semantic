#include "fft/world.h"
#include "psx/types.h"

/* Draw the command's two endpoints with its RGB color, then advance. */
world_menu_instruction_t* world_menu_script_draw_line(world_menu_instruction_t* instruction) {
    RECT rect;

    rect.x = instruction->line.x0;
    rect.y = instruction->line.y0;
    rect.w = instruction->line.x1;
    rect.h = instruction->line.y1;
    world_gfx_enqueue_line_f2(&rect, &instruction->line.color, g_world_menu_semi_trans, g_world_menu_draw_priority);
    return world_menu_next_instruction(instruction);
}
