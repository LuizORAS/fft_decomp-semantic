#include "fft/event_equip.h"
#include "psx/types.h"

/* EQUIP.OUT 001caf14 - Stream command: draw a LINE_F2 from (x0, y0) to
   (x1, y1) with the RGB that follows the endpoints in the command. */
menu_instruction_t* equip_cmd_draw_line_f2_handler(menu_instruction_t* stream) {
    RECT endpoints;

    endpoints.x = stream->line.x0;
    endpoints.y = stream->line.y0;
    endpoints.w = stream->line.x1;
    endpoints.h = stream->line.y1;
    equip_gfx_enqueue_line_f2(
        &endpoints, &stream->line.color, g_equip_gfx_semitransparency, g_equip_gfx_sprite_ot_index);
    return menu_next_instruction(stream);
}
