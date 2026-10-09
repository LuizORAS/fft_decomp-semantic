#include "fft/event_equip.h"
#include "psx/libc.h"
#include "psx/types.h"

/* Draw record: print a callback-supplied number right-aligned in a field of
 * `command[4]` eight-pixel cells, then advance past the record. `count` holds
 * the value, then the printed length, then the number of digit cells. */
u8* equip_cmd_draw_right_aligned_number_handler(u8* command) {
    s8 buf[0x10];
    RECT rect;
    s16 i;
    s16 x;
    s32 count;
    u8 y;
    s16 blanks;
    s32 j;

    count = g_equip_menu_list_row_callbacks[command[3]](command[9]);
    count = sprintf(buf, g_equip_text_decimal_format, count);
    blanks = command[4] - count;
    x = command[7];
    y = command[8];
    count = command[4] - blanks;
    g_equip_gfx_blank_cell_rect.y = y + 4;
    for (i = blanks - 1; i != -1; i--) {
        g_equip_gfx_blank_cell_rect.x = x;
        equip_gfx_enqueue_oriented_textured_quad(&g_equip_gfx_blank_cell_rect, 0, 0, 0, g_equip_gfx_sprite_ot_index);
        x += 8;
    }
    rect.x = x;
    rect.y = y;
    rect.w = 8;
    rect.h = 0x10;
    for (j = 0; j < count; j++) {
        equip_gfx_enqueue_textured_quad(&rect, (buf[j] * 8) - 0xD8, 0x30, 0, 0, g_equip_menu_cursor_texture_page,
            g_equip_text_clut_1_mode0, g_equip_gfx_sprite_ot_index);
        rect.x += 8;
    }
    return command + command[1];
}
