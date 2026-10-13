#include "fft/event_jobstts.h"
#include "fft/menu.h"

/* Draw a textured quad, unshaded for opcode 4. During list rendering,
 * offset its y coordinate by the current row and scroll position. */
menu_instruction_t* jobstts_cmd_draw_textured_quad_handler(menu_instruction_t* data) {
    RECT rect;
    s32 y;
    rgb24_t* color;

    if (g_jobstts_menu_list_row_rendering == 0) {
        y = data->sprite.sprite.y;
    } else {
        y = ((g_jobstts_menu_list_row_height * g_jobstts_menu_list_row_index) + data->sprite.sprite.y)
            - g_jobstts_menu_list_scroll_offset_y;
        if (g_jobstts_menu_list_scroll_offset_y < 0) {
            y -= g_jobstts_menu_list_row_height;
        }
    }
    rect.x = data->sprite.sprite.x;
    rect.y = y;
    rect.w = data->sprite.sprite.w;
    rect.h = data->sprite.sprite.h;
    color = &g_menu_text_color;
    if (data->header.opcode == MENU_SCRIPT_OP_UNCOLOURED_QUAD) {
        color = 0;
    }
    jobstts_gfx_enqueue_textured_quad(&rect, data->sprite.sprite.u, data->sprite.sprite.v, color,
        g_jobstts_gfx_semitransparency, g_menu_gfx_texture_page, g_menu_gfx_clut_id, g_jobstts_gfx_otag_index);
    return menu_next_instruction(data);
}
