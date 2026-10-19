#include "fft/event_jobstts.h"
#include "fft/menu.h"

/* Render command that draws the decimal value returned by one of the shared
 * condition/getter routines as a row of digit tiles.
 *
 * data->number.getter selects the routine in g_jobstts_menu_script_conditions,
 * width is the number of digit cells, spacing their horizontal pitch, x the left
 * edge and y the row offset; value supplies the routine argument outside a
 * scrollable list, and alternate_palette selects the window CLUT variant.
 * Inside a list row the argument and the row offset follow the same
 * scroll-base/row-height derivation as jobstts_menu_script_if_group.
 *
 * The routine returns flags in the top bits of its value: 0x40000000 selects
 * the highlighted CLUT, and 0x20000000 replaces the number with a row of
 * 6x3 dash tiles. Otherwise the value is formatted with sprintf and each
 * character indexes the 6x11 digit strip at (0x78, 0x10); leading cells are
 * blanked by drawing digit 0's cell, biased by the pad count.
 *
 * `dash_x` and `cell_x` exist because the target reads spacing and x before
 * storing the rectangle: those loads go through the command pointer, which
 * GCC 2.6.3 cannot disambiguate from the frame stores, so computing the x
 * coordinate first is what puts the loads ahead of the stores. They are two
 * variables and not one because the target gives the value a different
 * scratch register in each branch (v1 in the dash loop, a1 in the digit
 * loop); one shared variable becomes a global allocno and holds t2 in both.
 *
 * The dash loop reuses `value` as its counter, which is what keeps the
 * target's `move a2, s0`: a global allocno's copy is not coalesced, and a2 is
 * the register `value` already occupies at the 0x20000000 test. */
menu_instruction_t* jobstts_menu_script_draw_formatted_number(menu_instruction_t* data) {
    s8 text[16];
    menu_digit_glyph_t glyph;
    u16 unused[8]; /* Preserves the original frame size. */
    jobstts_menu_script_condition_t condition;
    s32 arg;
    s32 y;
    s32 value;
    /* Separate positions preserve the load/store order in the two loops. */
    s32 dash_x;
    s32 i;
    s32 pad;
    s32 pos;
    s32 digit;
    s32 cell_x;
    s32 cell;
    u16 clut;

    condition = g_jobstts_menu_script_conditions[data->number.getter];
    if (g_jobstts_menu_list_row_rendering == 0) {
        arg = data->number.value;
        y = data->number.y;
    } else {
        arg = g_jobstts_menu_scroll_base_index + g_jobstts_menu_list_row_index;
        y = ((g_jobstts_menu_list_row_height * g_jobstts_menu_list_row_index) + data->number.y)
            - g_jobstts_menu_list_scroll_offset_y;
        if (g_jobstts_menu_list_scroll_offset_y < 0) {
            arg -= 1;
            y -= g_jobstts_menu_list_row_height;
        }
    }

    value = condition(arg);
    if (data->number.alternate_palette != 0) {
        clut = g_menu_alternate_number_clut;
    } else {
        clut = g_menu_palette_clut;
    }
    if (value & MENU_NUMBER_VALUE_DISABLED) {
        clut = g_menu_disabled_number_clut;
        value &= ~MENU_NUMBER_VALUE_DISABLED;
    }

    glyph.u = 0x78;
    glyph.v = 0x10;
    glyph.w = 6;
    glyph.h = 0xB;

    if (value & MENU_NUMBER_VALUE_DASHES) {
        value = data->number.width;
        while (value > 0) {
            cell = value - 1;
            dash_x = data->number.x + (cell * data->number.spacing);
            glyph.rect.y = y + 5;
            glyph.rect.w = 6;
            glyph.rect.h = 3;
            glyph.rect.x = dash_x;
            jobstts_gfx_enqueue_textured_quad(&glyph.rect, 0xBA, 0x13, 0, g_jobstts_gfx_semitransparency,
                g_jobstts_gfx_background_texture_page, clut, g_menu_gfx_otag_index);
            value = cell;
        }
        return menu_next_instruction(data);
    }

    pad = data->number.width - sprintf(text, g_jobstts_text_decimal_format, value);
    pos = 0;
    i = 0;
    if (data->number.width != 0) {
        do {
            if (pad != 0) {
                digit = 0;
                pad -= 1;
            } else {
                digit = text[pos] - 0x30;
                pos += 1;
            }
            cell_x = data->number.x + (i * data->number.spacing);
            glyph.rect.w = glyph.w;
            glyph.rect.y = y;
            glyph.rect.h = glyph.h;
            glyph.rect.x = cell_x;
            jobstts_gfx_enqueue_textured_quad(&glyph.rect, glyph.u + (glyph.w * digit), glyph.v, 0,
                g_jobstts_gfx_semitransparency, g_jobstts_gfx_background_texture_page, clut, g_menu_gfx_otag_index);
            i += 1;
        } while (i < data->number.width);
    }
    return menu_next_instruction(data);
}
