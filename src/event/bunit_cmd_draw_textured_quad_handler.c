#include "fft/event_bunit.h"
#include "fft/menu.h"
#include "psx/types.h"

/* Draw a textured quad at its literal position or current list row, using
 * the current texture, palette and optional sprite color. */
menu_instruction_t* bunit_cmd_draw_textured_quad_handler(menu_instruction_t* cmd) {
    RECT rect;
    s32 index;
    rgb24_t* table;

    if (g_bunit_menu_scroll_list_active == 0) {
        index = cmd->sprite.sprite.y;
    } else {
        index = g_bunit_menu_list_row_height * g_bunit_menu_cursor_row + cmd->sprite.sprite.y
            - g_bunit_menu_scroll_pixel_offset;
        if (g_bunit_menu_scroll_pixel_offset < 0) {
            index -= g_bunit_menu_list_row_height;
        }
    }

    rect.x = cmd->sprite.sprite.x;
    rect.y = index;
    rect.w = cmd->sprite.sprite.w;
    rect.h = cmd->sprite.sprite.h;

    table = &g_bunit_gfx_sprite_color;
    if (cmd->header.opcode == MENU_SCRIPT_OP_UNCOLOURED_QUAD) {
        table = 0;
    }

    menu_gfx_enqueue_textured_quad(&rect, cmd->sprite.sprite.u, cmd->sprite.sprite.v, table,
        g_bunit_gfx_semitrans_enabled, g_menu_gfx_texture_page, g_menu_gfx_clut_id, g_bunit_gfx_otag_index);
    return menu_next_instruction(cmd);
}
