#include "fft/event_card.h"
#include "psx/types.h"

/* Stream command: draw a textured quad of size (command[5], command[6]) at
 * (command[3], command[4]) with texture origin (command[7], command[8]), in the
 * current sprite colour or, for MENU_SCRIPT_OP_UNCOLOURED_QUAD, unshaded. Returns the next command. */
u8* card_menu_script_draw_textured_quad(u8* command) {
    RECT rect;
    const u8* color;
    s32 x;
    s32 y;

    x = command[3];
    y = command[4];
    rect.x = x;
    rect.y = y;
    rect.w = command[5];
    rect.h = command[6];
    color = g_card_text_color;
    if (command[0] == MENU_SCRIPT_OP_UNCOLOURED_QUAD) {
        color = 0;
    }
    card_gfx_enqueue_textured_quad(&rect, command[7], command[8], color, g_card_gfx_draw_semitrans,
        g_card_gfx_texture_page, g_card_gfx_draw_clut, g_card_text_otag_index);
    return command + command[1];
}
