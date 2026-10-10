#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Numeric entry thread: edits the halfword at param->value one decimal digit
 * at a time within the range bound at param->text_binding. Left/right move the digit cursor, up/down
 * step the value by the g_world_menu_number_entry_digit_steps step of that digit (clamped to the range),
 * and a changed value is redrawn as a signed number in the window. When the
 * cancel/confirm handlers change the input word, a -1 left in the value
 * restores the original; otherwise the value is stored and the range's
 * on_change callback runs. */
void world_menu_number_entry_thread(void) {
    RECT rect;
    world_menu_icon_record_t records[2];
    POLY_FT4 cursor_quads[2];
    POLY_FT4 shadow_quads[2];
    world_menu_number_range_t* range;
    void* buffer;
    s16 original;
    s32 digits;
    native_thread_t* thread;
    world_menu_entry_t* param;
    s16 value;
    s32 i;
    s32 j;
    s32 digit;
    s32 redraw;
    u32 input;
    world_menu_icon_sprites_t* record;
    POLY_FT4* cursor_quad;
    POLY_FT4* shadow_quad;
    s32* stride;

    g_world_menu_list_controller_input = world_input_get_menu_controller(0);
    /* A register copy of the address: the target forms the pen address as
     * &g_world_menu_text_state.pen.stride - 8 instead of folding the constant. */
    stride = &g_world_menu_text_state.pen.stride;
    thread = &g_world_threads[g_world_thread_current_id];
    /* Read as a plain word rather than thread->function_parameter_1: GCC 2.6.3
     * lets a struct-member load pass the g_world_menu_list_controller_input store above, but the
     * target keeps the store first. */
    param = *(world_menu_entry_t**)thread;
    range = (world_menu_number_range_t*)param->text_binding;
    digits = world_text_count_decimal_digits(range->max);
    original = *param->value;
    value = original;
    if (value < range->min || range->max < value) {
        value = range->min;
    }
    world_menu_build_icon_record(&rect, param, &records[0]);
    world_script_copy_bytes(&records[1], &records[0], 0x7C);
    for (j = 0; j < 2; j++) {
        world_gfx_init_default_poly_ft4(&cursor_quads[j]);
        SetSemiTrans(&cursor_quads[j], 0);
        cursor_quads[j].tpage = GetTPage(0, 0, 0x3C0, 0x100);
        cursor_quads[j].clut = 0x7D7C;
        cursor_quads[j].u0 = 0xB7;
        cursor_quads[j].u1 = 0xB7;
        cursor_quads[j].u2 = 0xA7;
        cursor_quads[j].u3 = 0xA7;
        cursor_quads[j].v0 = 0;
        cursor_quads[j].v1 = 0x10;
        cursor_quads[j].v2 = 0;
        cursor_quads[j].v3 = 0x10;
        world_gfx_init_default_poly_ft4(&shadow_quads[j]);
        SetSemiTrans(&shadow_quads[j], 1);
        shadow_quads[j].tpage = GetTPage(0, 2, 0x3C0, 0x100);
        shadow_quads[j].clut = 0x7DBC;
        shadow_quads[j].u0 = 0xC7;
        shadow_quads[j].u1 = 0xC7;
        shadow_quads[j].u2 = 0xB7;
        shadow_quads[j].u3 = 0xB7;
        shadow_quads[j].v0 = 0;
        shadow_quads[j].v1 = 0x10;
        shadow_quads[j].v2 = 0;
        shadow_quads[j].v3 = 0x10;
    }
    digit = 0;
    buffer = world_menu_build_and_upload_window_frame_image(
        param->inner_width, param->inner_height, &rect, MENU_WINDOW_FRAME_BUILD);
    *stride = param->inner_width;
    world_menu_set_text_origin(8, 9);
    world_menu_set_text_origin(param->inner_width - digits * 6 - 0x10, 9);
    world_gfx_draw_signed_number(value, digits, buffer, (world_glyph_blit_t*)(stride - 2));
    LoadImage(&rect, buffer);
    redraw = 1;
    for (i = 0;; i++) {
        record = &records[i & 1].base;
        *param->value = value;
        world_thread_wait_frames(1);
        if (redraw == 1) {
            redraw = 0;
            world_menu_free_memory(buffer);
        }
        if (world_menu_check_thread_completion(g_world_menu_list_controller_input) != 0) {
            break;
        }
        input = *g_world_menu_list_controller_input;
        world_menu_cancel_thread_group(param);
        world_menu_handle_entry_confirm(param, 0);
        if (input != *g_world_menu_list_controller_input) {
            if (*param->value == -1) {
                *param->value = original;
            } else {
                *param->value = value;
                ((world_menu_number_range_t*)param->text_binding)->on_change();
            }
        }
        if (thread[-1].is_running == 0) {
            if ((*g_world_menu_list_controller_input & PSX_PAD_RIGHT) && digit > 0) {
                digit--;
            }
            if ((*g_world_menu_list_controller_input & PSX_PAD_LEFT) && digit < digits - 1) {
                digit++;
            }
            if (*g_world_menu_list_controller_input & PSX_PAD_UP) {
                if (value <= range->max - g_world_menu_number_entry_digit_steps[digit]) {
                    value += g_world_menu_number_entry_digit_steps[digit];
                } else {
                    value = range->max;
                }
                redraw = 1;
            }
            if (*g_world_menu_list_controller_input & PSX_PAD_DOWN) {
                if (value >= range->min + g_world_menu_number_entry_digit_steps[digit]) {
                    value -= g_world_menu_number_entry_digit_steps[digit];
                } else {
                    value = range->min;
                }
                redraw = 1;
            }
        }
        if (redraw == 1) {
            buffer = world_menu_build_and_upload_window_frame_image(
                param->inner_width, param->inner_height, &rect, MENU_WINDOW_FRAME_BUILD);
            *stride = param->inner_width;
            world_menu_set_text_origin(param->inner_width - digits * 6 - 0x10, 9);
            world_gfx_draw_signed_number(value, digits, buffer, (world_glyph_blit_t*)(stride - 2));
            LoadImage(&rect, buffer);
        }
        cursor_quad = &cursor_quads[i & 1];
        shadow_quad = &shadow_quads[i & 1];
        world_menu_update_entry_cursor_quads(param, cursor_quad, shadow_quad, i, digit);
        world_menu_select_sprite_clut(cursor_quad);
        world_menu_select_sprite_clut(shadow_quad);
        if (world_thread_is_previous_running() == 0) {
            cursor_quad->clut = 0x7D7C;
            shadow_quad->clut = 0x7DBC;
        } else {
            cursor_quad->clut = 0x7DFC;
            shadow_quad->clut = 0x7E3C;
        }
        world_menu_update_icon_cursor_sprites(param, record, i, -1);
        world_menu_select_icon_cluts(record);
        world_gfx_draw_or_append_gpu_primitive(&cursor_quads[i & 1]);
        world_gfx_draw_or_append_gpu_primitive(&shadow_quads[i & 1]);
        world_gfx_draw_or_append_gpu_primitive(&record->sprites[0]);
        world_gfx_draw_or_append_gpu_primitive(record);
    }
    world_thread_yield();
    world_thread_exit_current();
}
