#include "fft/world.h"
#include "psx/types.h"

#define TEXT_STATE ((text_backreference_state_t*)0x1F800200)

/* Render a WORLD text entry glyph by glyph into a menu window image.
 *
 * `text` of 0 or 1 selects `text_id` from the text tables; 1 also lets the
 * 0xFB command continue instead of ending the text. Walks the same command
 * stream as world_text_measure, drawing lines from g_world_menu_text_param_22
 * up to g_world_menu_text_param_24 and stopping at the window's right margin.
 * Numbers (0xE4/0xE6) are drawn one digit per 6-pixel column. Indexing the
 * divisor table inside the digit loop, rather than walking a pointer, is
 * what gives the target's strength-reduced pointer and register choice. */
void world_menu_display_text(s32 text_id, u8* pixels, glyph_blit_t* origin, u8* text) {
    party_data_t* party;
    u8* cursor;
    s32 i;
    u8* fill = g_world_text_player_name_buffer;
    u8 terminator;
    text_backreference_state_t* state;
    s32 variable_id;
    s32 line;
    s32 started;

    party = main_party_get_data_pointer(0);
    terminator = TEXT_END_WAIT_FOR_CONFIRM;
    for (i = 19; i >= 0; i--) {
        fill[i] = terminator;
    }
    for (i = 0; i < 16; i++) {
        g_world_text_player_name_buffer[i] = party->name[i];
    }
    if (text == 0 || text == (u8*)1) {
        cursor = world_text_find_entry(text_id);
    } else {
        cursor = text;
    }
    state = TEXT_STATE;
    variable_id = EVENT_SCRIPT_VAR_SELECTED_DIALOG_OPTION;
    TEXT_STATE->return_cursor = (u8*)-1;
    TEXT_STATE->value_cursor = g_world_text_substitution_values;
    TEXT_STATE->pending_width = 0xFFFF;
    TEXT_STATE->remaining_bytes = 0;
    TEXT_STATE->column = 0;
    TEXT_STATE->max_column = 0;
    TEXT_STATE->row = 0;
    g_world_text_draw_origin.rect.x = origin->rect.x;
    g_world_text_glyph_first_row = 0;
    g_world_text_draw_origin.rect.y = origin->rect.y;
    g_world_text_glyph_row_limit = 0x10;
    g_world_text_draw_origin.stride = origin->stride;
    line = 0;

    while (1) {
        state->command = *cursor;
        if (state->command >= TEXT_EXTENDED_GLYPH_PREFIX_FIRST) {
            if ((state->command & 0xF0) == TEXT_EXTENDED_GLYPH_PREFIX_FIRST) {
                state->command = (*cursor & 0xF) * TEXT_GLYPHS_PER_PREFIX;
                cursor = world_menu_text_advance_cursor_with_backreferences(state, cursor);
                state->command = *cursor + state->command;
            } else {
                if (state->command == TEXT_SET_GLYPH_DELAY) {
                    cursor = world_menu_text_advance_cursor_with_backreferences(state, cursor);
                } else if (state->command == TEXT_SET_PORTRAIT) {
                    cursor = world_menu_text_advance_cursor_with_backreferences(state, cursor);
                    world_script_set_variable(EVENT_SCRIPT_VAR_DIALOG_PORTRAIT, *cursor);
                } else if (state->command == TEXT_SET_VARIABLE_BASE) {
                    cursor = world_menu_text_advance_cursor_with_backreferences(state, cursor);
                    variable_id = *cursor;
                } else if (state->command == TEXT_STORE_VARIABLE) {
                    cursor = world_menu_text_advance_cursor_with_backreferences(state, cursor);
                    i = world_script_get_variable(EVENT_SCRIPT_VAR_TEXT_STORE_INDEX);
                    if (i != 0) {
                        world_script_set_variable(variable_id + i * 10, *cursor);
                        world_script_set_variable(EVENT_SCRIPT_VAR_TEXT_STORE_INDEX, 0);
                    } else {
                        world_script_set_variable(variable_id, *cursor);
                    }
                } else if (state->command == TEXT_SET_PALETTE) {
                    cursor = world_menu_text_advance_cursor_with_backreferences(state, cursor);
                    g_world_menu_text_color = *cursor;
                } else if (state->command == TEXT_CHOICE_MARK) {
                    if (text != (u8*)1) {
                        break;
                    }
                } else if (state->command == TEXT_SPACE) {
                    g_world_text_draw_origin.rect.x += 4;
                } else if (state->command == TEXT_NEWLINE) {
                    g_world_text_draw_origin.rect.x = origin->rect.x;
                    if (line >= g_world_menu_text_param_22) {
                        g_world_text_glyph_first_row = 0;
                        g_world_text_draw_origin.rect.y += 0x10;
                    }
                    line++;
                } else if (state->command == TEXT_SET_NUMBER_WIDTH) {
                    cursor = world_menu_text_advance_cursor_with_backreferences(state, cursor);
                    state->pending_width = *cursor;
                } else if (state->command == TEXT_PRINT_NEXT_VALUE || state->command == TEXT_PRINT_INDEXED_VALUE) {
                    if (state->command == TEXT_PRINT_NEXT_VALUE) {
                        state->value = *state->value_cursor;
                        state->value_cursor++;
                    } else {
                        cursor = world_menu_text_advance_cursor_with_backreferences(state, cursor);
                        state->value = g_world_text_substitution_values[*cursor];
                    }
                    i = state->pending_width - 1;
                    if (state->pending_width == 0xFFFF) {
                        started = 0;
                        i = world_text_count_decimal_digits(state->value) - 1;
                    } else {
                        state->pending_width = 0xFFFF;
                        started = 0;
                    }
                    for (; i >= 0; i--) {
                        if (state->value >= g_world_text_decimal_divisors[i] || i == 0 || started != 0) {
                            state->command = state->value / g_world_text_decimal_divisors[i];
                            started = 1;
                            state->value = state->value - state->command * g_world_text_decimal_divisors[i];
                            if (line >= g_world_menu_text_param_24) {
                                /* Leaves the digit loop and the text loop. */
                                goto end;
                            }
                            if (line >= g_world_menu_text_param_22) {
                                world_text_blit_font_glyph_to_4bpp(
                                    g_world_text_glyph_bitmap_data + state->command * TEXT_GLYPH_BYTES, pixels,
                                    &g_world_text_draw_origin, g_world_menu_text_color);
                            }
                        }
                        g_world_text_draw_origin.rect.x += 6;
                    }
                }

                state->value = world_text_resolve_format_string_id(state->command, *state->value_cursor);
                if (state->value != -1) {
                    state->return_cursor = cursor + 1;
                    if (state->command == TEXT_FORMAT_RAMZA_NAME) {
                        cursor = g_world_text_player_name_buffer;
                    } else {
                        cursor = world_text_find_entry(state->value);
                    }
                    state->value_cursor++;
                    continue;
                }
                if ((state->command & 0xF0) == TEXT_BACKREFERENCE_PREFIX_FIRST && (state->command & 0xF) < 4) {
                    state->return_cursor = cursor + 3;
                    state->scratch = (cursor[0] & 3) * 8;
                    state->remaining_bytes = cursor[1] >> 5;
                    state->remaining_bytes = state->remaining_bytes + state->scratch + 4;
                    state->scratch = (cursor[1] & 0xF) * 0xFE;
                    state->scratch = cursor[2] + state->scratch;
                    cursor -= state->scratch;
                    continue;
                }
                if ((state->command & 0xFE) == 0xFE) {
                    if (state->return_cursor == (u8*)-1) {
                        break;
                    }
                    cursor = state->return_cursor;
                    state->return_cursor = (u8*)-1;
                    continue;
                }
                cursor = world_menu_text_advance_cursor_with_backreferences(state, cursor);
                continue;
            }
        }
        cursor = world_menu_text_advance_cursor_with_backreferences(state, cursor);
        if (line >= g_world_menu_text_param_24) {
            break;
        }
        if (line >= g_world_menu_text_param_22) {
            world_text_blit_font_glyph_to_4bpp(g_world_text_glyph_bitmap_data + state->command * TEXT_GLYPH_BYTES,
                pixels, &g_world_text_draw_origin, g_world_menu_text_color);
        }
        g_world_text_draw_origin.rect.x += g_world_text_glyph_widths[state->command];
        if (g_world_text_draw_origin.rect.x >= g_world_text_draw_origin.stride - 8) {
            break;
        }
    }
end:
    g_world_menu_text_color = 0;
    g_world_menu_text_param_22 = 0;
    g_world_menu_text_param_24 = 0xFF;
}
