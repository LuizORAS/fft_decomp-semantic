#include "fft/world.h"
#include "psx/types.h"

/* Menu-script opcode handler: bytes 2..5 give the line's start x, y and end
 * x, y (passed in a RECT's x, y, w, h); the colour payload starts at byte 6.
 * Returns the address of the next instruction. */
u8* world_menu_script_draw_line(u8* script) {
    RECT rect;

    rect.x = script[2];
    rect.y = script[3];
    rect.w = script[4];
    rect.h = script[5];
    world_gfx_enqueue_line_f2(&rect, script + 6, g_world_menu_semi_trans, g_world_menu_draw_priority);
    return script + script[1];
}
