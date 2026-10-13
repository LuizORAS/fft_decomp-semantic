#include "fft/event_equip.h"
#include "psx/types.h"

/* Opcode 7 (MENU_SCRIPT_OP_LINE): queue a flat line (equip_gfx_enqueue_line_f2) from (byte 2, byte 3)
 * to (byte 4, byte 5) in the RGB colour at byte 6, with the current semi-transparency and draw priority.
 * Returns the next command. */
u8* equip_menu_script_draw_line(u8* stream) {
    RECT endpoints;

    endpoints.x = stream[2];
    endpoints.y = stream[3];
    endpoints.w = stream[4];
    endpoints.h = stream[5];
    equip_gfx_enqueue_line_f2(&endpoints, stream + 6, g_equip_gfx_semitransparency, g_equip_gfx_sprite_ot_index);
    return stream + stream[1];
}
