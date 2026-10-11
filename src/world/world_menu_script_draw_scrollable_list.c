#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/pad.h"
#include "psx/types.h"

/*
 * Menu-script handler for a scrollable list: the WORLD original of the
 * overlays' scrollable-list body commands.
 *
 * Moves the cursor on Up/Down (bits 0x10000000 and 0x40000000 of the script
 * input, the held-repeat flags of Up and Down, keep a scroll going without
 * moving the cursor) and pages through
 * world_menu_scroll_list_page. Moving past the first or last visible row
 * starts a one-row scroll: the newly exposed row is rendered into the list's
 * VRAM strip at x 0x240, the pixel offset advances by the slow step until the
 * repeat timer reaches g_main_menu_scroll_accel_delay and by the fast step
 * after it, and when a full row height is covered the strip is moved one row
 * with draw-move primitives (one per row when scrolling up). Then it draws the list image, the up and down
 * arrows, the scrollbar thumb and the cursor, and replays the per-row record
 * group once per visible row, plus one while a scroll is in progress.
 *
 * An empty list only skips record->skip_count + 1 chained records. Returns
 * the record that follows the list.
 */
u8* world_menu_script_draw_scrollable_list(world_menu_list_record_t* record) {
    RECT rect;
    s32 unused[4];
    point16_t point;
    u16 priority;
    u8 speed;
    s32 i;
    s32 j;
    s32 rows;
    s32 index;
    u16 old_cursor;
    world_menu_list_record_t* base;

    priority = g_world_menu_draw_priority;
    if (g_world_menu_option_count <= 0) {
        for (j = record->skip_count; j != -1; j--) {
            record = (world_menu_list_record_t*)world_skip_chained_records((world_chained_record_t*)record);
        }
        return (u8*)record;
    }
    speed = world_gfx_get_vsync_mode_or_one();
    g_world_menu_use_scroll_position = 1;
    if (g_world_menu_list_layout_pending != 0) {
        world_menu_load_scrollable_list_layout((u8*)record);
    }
    old_cursor = g_world_menu_cursor_position;
    if (g_world_menu_list_scroll_direction == 0) {
        i = world_input_read_menu_scroll_repeat();
        if (i != 0) {
            if (i != 2) {
                world_menu_scroll_list_page(i, record);
            }
        } else if ((g_world_menu_display_script_input & 0x10000000)
            || (g_world_menu_display_script_input & PSX_PAD_UP)) {
            index = g_world_menu_scroll_offset;
            if (index == g_world_menu_cursor_position) {
                if (index > 0) {
                    g_world_menu_list_scroll_direction = -1;
                    if (g_world_menu_list_text_table != 0) {
                        g_world_menu_list_row_text_ids[0] = g_world_menu_list_entry_ids[index - 1];
                        rect.x = 0x240;
                        rect.y = 0x120;
                        rect.w = g_world_menu_list_image_width;
                        rect.h = g_world_menu_row_height;
                        world_text_render_id_list_to_image_rows(
                            g_world_menu_list_text_table, g_world_menu_list_row_text_ids, &rect, 0);
                    }
                }
            } else if (g_world_menu_display_script_input & PSX_PAD_UP) {
                g_world_menu_cursor_position--;
            }
        } else if ((g_world_menu_display_script_input & 0x40000000)
            || (g_world_menu_display_script_input & PSX_PAD_DOWN)) {
            if (g_world_menu_scroll_offset + g_world_menu_list_visible_rows - 1 == g_world_menu_cursor_position) {
                if (g_world_menu_scroll_offset + g_world_menu_list_visible_rows - 1 < g_world_menu_option_count - 1) {
                    g_world_menu_list_scroll_direction = 1;
                    if (g_world_menu_list_text_table != 0) {
                        index = g_world_menu_scroll_offset + g_world_menu_list_visible_rows - 1;
                        g_world_menu_list_row_text_ids[0] = g_world_menu_list_entry_ids[index + 1];
                        rect.x = 0x240;
                        rect.y = g_world_menu_list_visible_rows * g_world_menu_row_height + 0x130;
                        rect.w = g_world_menu_list_image_width;
                        rect.h = g_world_menu_row_height;
                        world_text_render_id_list_to_image_rows(
                            g_world_menu_list_text_table, g_world_menu_list_row_text_ids, &rect, 0);
                    }
                }
            } else {
                if (g_world_menu_cursor_position < g_world_menu_option_count - 1
                    && (g_world_menu_display_script_input & PSX_PAD_DOWN)) {
                    g_world_menu_cursor_position++;
                }
            }
        } else {
            g_world_menu_scroll_repeat_timer = 0;
        }
    }
    if (g_world_menu_list_scroll_direction < 0) {
        if (g_world_menu_scroll_repeat_timer >= g_main_menu_scroll_accel_delay) {
            g_world_menu_scroll_pixel_offset -= g_main_menu_scroll_fast_step * speed;
        } else {
            g_world_menu_scroll_repeat_timer += speed;
            g_world_menu_scroll_pixel_offset -= g_main_menu_scroll_slow_step * speed;
        }
        if (g_world_menu_scroll_pixel_offset <= -g_world_menu_row_height) {
            g_world_menu_scroll_pixel_offset = 0;
            g_world_menu_list_scroll_direction = 0;
            g_world_menu_scroll_offset = --g_world_menu_cursor_position;
            if (g_world_menu_list_text_table != 0) {
                for (i = 0; i < g_world_menu_list_visible_rows; i++) {
                    rect.x = 0x240;
                    rect.y = (i - 1) * g_world_menu_row_height + 0x130;
                    rect.w = g_world_menu_list_image_width;
                    rect.h = g_world_menu_row_height;
                    world_gfx_enqueue_draw_move(&rect, 0x240, g_world_menu_row_height * i + 0x130, priority - 1);
                }
            }
        }
    } else if (g_world_menu_list_scroll_direction > 0) {
        if (g_world_menu_scroll_repeat_timer >= g_main_menu_scroll_accel_delay) {
            g_world_menu_scroll_pixel_offset += g_main_menu_scroll_fast_step * speed;
        } else {
            g_world_menu_scroll_repeat_timer += speed;
            g_world_menu_scroll_pixel_offset += g_main_menu_scroll_slow_step * speed;
        }
        if (g_world_menu_scroll_pixel_offset >= g_world_menu_row_height) {
            g_world_menu_scroll_pixel_offset = 0;
            g_world_menu_list_scroll_direction = 0;
            g_world_menu_scroll_offset++;
            g_world_menu_cursor_position++;
            if (g_world_menu_list_text_table != 0) {
                rect.x = 0x240;
                rect.y = g_world_menu_row_height + 0x130;
                rect.w = g_world_menu_list_image_width;
                rect.h = g_world_menu_row_height * g_world_menu_list_visible_rows;
                world_gfx_enqueue_draw_move(&rect, 0x240, 0x130, priority - 1);
            }
        }
    }
    if (g_world_menu_list_text_table != 0) {
        if (g_world_menu_scroll_pixel_offset != 0) {
            rect.x = record->x;
            rect.y = record->y - 5;
            rect.w = g_world_menu_list_image_width * 4;
            rect.h = g_world_menu_row_height * g_world_menu_list_visible_rows + 10;
            world_gfx_enqueue_textured_quad(&rect, 0, g_world_menu_scroll_pixel_offset + 0x2B, 0,
                g_world_menu_semi_trans, g_world_menu_window_tpage, g_world_menu_window_clut,
                g_world_menu_draw_priority);
        } else {
            rect.x = record->x;
            rect.y = record->y;
            rect.w = g_world_menu_list_image_width * 4;
            rect.h = g_world_menu_row_height * g_world_menu_list_visible_rows;
            world_gfx_enqueue_textured_quad(&rect, 0, 0x30, 0, g_world_menu_semi_trans, g_world_menu_window_tpage,
                g_world_menu_window_clut, g_world_menu_draw_priority);
        }
    }
    if (g_world_menu_use_alternate_palette == 0) {
        if (g_world_menu_scroll_offset != 0) {
            rect.x = g_world_menu_window_right_x;
            rect.y = record->arrow_up_y;
            rect.w = 8;
            rect.h = 0x10;
            world_gfx_enqueue_textured_quad(&rect, 0xD8, 0, 0, g_world_menu_semi_trans, g_world_menu_icon_tpage,
                g_world_menu_clut_front, g_world_menu_draw_priority);
        }
        if (g_world_menu_scroll_offset < g_world_menu_option_count - g_world_menu_list_visible_rows) {
            rect.x = g_world_menu_window_right_x;
            rect.y = record->arrow_down_y;
            rect.w = 8;
            rect.h = 0x10;
            world_gfx_enqueue_textured_quad(&rect, 0xE0, 0, 0, g_world_menu_semi_trans, g_world_menu_icon_tpage,
                g_world_menu_clut_front, g_world_menu_draw_priority);
        }
        if (g_world_menu_option_count > g_world_menu_list_visible_rows) {
            i = record->arrow_up_y + 0xF;
            j = record->arrow_down_y - (s16)(i + 5);
            i += j * g_world_menu_cursor_position / (g_world_menu_option_count - 1);
            rect.x = g_world_menu_window_right_x;
            rect.y = i;
            rect.w = 8;
            rect.h = 8;
            world_gfx_enqueue_textured_quad(&rect, 0x10, 0x10, 0, g_world_menu_semi_trans, g_world_menu_icon_tpage,
                g_world_menu_clut_front, g_world_menu_draw_priority);
        }
    }
    if (record->show_cursor != 0) {
        point.x = record->x - record->cursor_x_offset;
        point.y = record->y + (g_world_menu_cursor_position - g_world_menu_scroll_offset) * g_world_menu_row_height;
        world_menu_draw_animated_cursor(&point, &g_world_menu_list_cursor_anim, g_world_menu_use_alternate_palette);
    }
    record = (world_menu_list_record_t*)((u8*)record + record->length);
    g_world_menu_scroll_row_offset = 0;
    rows = g_world_menu_list_visible_rows + 1;
    if (g_world_menu_scroll_pixel_offset == 0) {
        rows--;
    }
    base = record;
    for (i = 0; i < rows; i++, g_world_menu_scroll_row_offset++) {
        if (g_world_menu_scroll_offset + i > g_world_menu_option_count - 1 && g_world_menu_scroll_pixel_offset == 0) {
            break;
        }
        record = base;
        for (j = 0; j < g_world_menu_list_row_record_count; j++) {
            record = (world_menu_list_record_t*)g_world_menu_script_handlers[record->type]((u8*)record);
        }
    }
    g_world_menu_use_scroll_position = 0;
    if (old_cursor != g_world_menu_cursor_position) {
        g_world_menu_sound_effect_id = MAIN_SFX_CURSOR_MOVE;
    }
    return (u8*)record;
}
