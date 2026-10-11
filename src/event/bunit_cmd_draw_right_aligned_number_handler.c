#include "fft/event_bunit.h"
#include "psx/libc.h"
#include "psx/types.h"

/* Draw a callback value right-aligned in eight-pixel cells. */
menu_instruction_t* bunit_cmd_draw_right_aligned_number_handler(menu_instruction_t* cmd) {
    s8 digits[16];
    RECT glyph_rect;
    s32 length;
    s32 count;
    s16 blanks;
    s16 i;
    s32 j;
    s32 x;
    s32 y;

    length = sprintf(
        digits, g_bunit_text_decimal_format, g_bunit_menu_row_callbacks[cmd->number.getter](cmd->number.value));
    x = cmd->number.x;
    y = cmd->number.y;
    blanks = cmd->number.width - length;
    count = cmd->number.width - blanks;
    g_bunit_gfx_blank_cell_rect.position.y = y + 4;
    for (i = blanks - 1; i != -1; i--) {
        g_bunit_gfx_blank_cell_rect.position.x = x;
        bunit_gfx_enqueue_oriented_textured_quad(
            &g_bunit_gfx_blank_cell_rect, 0, TEXTURE_UV_FLIP_NONE, 0, g_bunit_gfx_otag_index);
        x += 8;
    }
    glyph_rect.x = x;
    glyph_rect.y = y;
    glyph_rect.w = 8;
    glyph_rect.h = 0x10;
    for (j = 0; j < count; j++) {
        bunit_gfx_enqueue_textured_quad(&glyph_rect, (digits[j] - '0') * 8 + 0xA8, 0x30, 0, 0,
            g_bunit_menu_icon_texture_page, 0x3ED3, g_bunit_gfx_otag_index);
        glyph_rect.x += 8;
    }
    /* Removing this barrier changes count/command register allocation. */
    __asm__("" : : "r"(count), "r"(count), "r"(count), "r"(count));
    return menu_next_instruction(cmd);
}
