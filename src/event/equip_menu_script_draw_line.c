#include "fft/event_equip.h"
#include "fft/menu.h"

/* Opcode 7 (MENU_SCRIPT_OP_LINE): queue a flat line (equip_gfx_enqueue_line_f2) from (byte 2, byte 3)
 * to (byte 4, byte 5) in the RGB colour at byte 6, with the current semi-transparency and draw priority.
 * Returns the next command. */
menu_instruction_t* equip_menu_script_draw_line(menu_instruction_t* stream) {
    point16_t endpoints[2];

    endpoints[0].x = stream->line.x0;
    endpoints[0].y = stream->line.y0;
    endpoints[1].x = stream->line.x1;
    endpoints[1].y = stream->line.y1;
    equip_gfx_enqueue_line_f2(endpoints, &stream->line.color, g_menu_gfx_semitransparency, g_menu_gfx_otag_index);
    return menu_next_instruction(stream);
}
