#include "fft/event_jobstts.h"

/* Stream command: draw a textured quad of size (data[5], data[6]) at
 * (data[3], data[4]) with texture origin (data[7], data[8]), in the
 * current sprite colour or, for MENU_SCRIPT_OP_UNCOLOURED_QUAD, unshaded. While
 * a list row is rendered, y is offset by the row's position and the scroll
 * offset. Returns the next command. */
u8* jobstts_cmd_draw_textured_quad_handler(u8* data) {
    RECT rect;
    s32 y;
    rgb24_t* color;

    if (g_jobstts_menu_list_row_rendering == 0) {
        y = data[4];
    } else {
        y = ((g_jobstts_menu_list_row_height * g_jobstts_menu_list_row_index) + data[4])
            - g_jobstts_menu_list_scroll_offset_y;
        if (g_jobstts_menu_list_scroll_offset_y < 0) {
            y -= g_jobstts_menu_list_row_height;
        }
    }
    rect.x = data[3];
    rect.y = y;
    rect.w = data[5];
    rect.h = data[6];
    color = &g_jobstts_text_color;
    if (data[0] == MENU_SCRIPT_OP_UNCOLOURED_QUAD) {
        color = 0;
    }
    jobstts_gfx_enqueue_textured_quad(&rect, data[7], data[8], color, g_jobstts_gfx_semitransparency,
        g_jobstts_gfx_texture_page, g_jobstts_gfx_clut_id, g_jobstts_gfx_otag_index);
    return data + data[1];
}
