#include "fft/event_equip.h"
#include "fft/menu.h"
#include "fft/menu_script.h"
#include "psx/types.h"

/* Stream command: draw a textured quad at its position, size and texture
 * origin (cmd->sprite), in the current sprite color or, for
 * MENU_SCRIPT_OP_UNCOLOURED_QUAD, unshaded. While a list row is rendered, y is
 * offset by the row's position and the scroll offset. Returns the next command. */
menu_instruction_t* menu_script_draw_textured_quad(menu_instruction_t* cmd) {
    RECT rect;
    s32 y;
    rgb24_t* color;

    if (g_menu_scroll_list_active == 0) {
        y = cmd->sprite.sprite.y;
    } else {
        y = g_equip_menu_list_row_height * g_menu_list_row_index + cmd->sprite.sprite.y - g_menu_list_scroll_offset;
        if (g_menu_list_scroll_offset < 0) {
            y -= g_equip_menu_list_row_height;
        }
    }

    rect.x = cmd->sprite.sprite.x;
    rect.y = y;
    rect.w = cmd->sprite.sprite.w;
    rect.h = cmd->sprite.sprite.h;

    color = &g_menu_gfx_sprite_color;
    if (cmd->header.opcode == MENU_SCRIPT_OP_UNCOLOURED_QUAD) {
        color = 0;
    }

    menu_gfx_enqueue_textured_quad(&rect, cmd->sprite.sprite.u, cmd->sprite.sprite.v, color,
        g_menu_gfx_semitransparency, g_menu_gfx_texture_page, g_menu_gfx_clut_id, g_menu_gfx_otag_index);
    return menu_next_instruction(cmd);
}
