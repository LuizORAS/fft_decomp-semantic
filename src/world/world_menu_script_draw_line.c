#include "fft/world.h"

/* Menu-script opcode handler: bytes 2..5 give the line's start x, y and end
 * x, y (passed in a RECT's x, y, w, h); the color payload starts at byte 6.
 * Returns the address of the next instruction. */
menu_instruction_t* world_menu_script_draw_line(menu_instruction_t* instruction) {
    point16_t endpoints[2];

    endpoints[0].x = instruction->line.x0;
    endpoints[0].y = instruction->line.y0;
    endpoints[1].x = instruction->line.x1;
    endpoints[1].y = instruction->line.y1;
    world_gfx_enqueue_line_f2(endpoints, &instruction->line.color, g_world_menu_semi_trans, g_world_menu_draw_priority);
    return menu_next_instruction(instruction);
}
