#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Thread task 0x3b: numeric editor panel, the WORLD twin of
 * attack_editor_run_numeric_thread (also in REQUIRE and DEBUGCHR). Thread 0xc
 * edits unsigned entries, other threads signed ones, each in its own
 * double-buffered battle_menu_status_panel_numeric_buffer_t. The only code difference from
 * the event overlays is the draw offset: WORLD reads the 0/0xf0 field value
 * directly from g_world_frame_arg (lhu) where the overlays compute
 * g_main_gfx_screen_polarity * 0xf0. */
void world_menu_run_numeric_editor_thread(void) {
    world_status_thread_t* thread;
    battle_menu_status_panel_numeric_buffer_t* buffer;
    battle_menu_status_panel_numeric_buffer_t* buffers;
    u8* text_pixels;
    menu_number_entry_t* entries;
    RECT* upload_a;
    RECT* upload_b;
    RECT* upload_c;
    world_gfx_image_load_parameters_t* descriptor;
    s32 i;
    s32 frame;
    s32 field_y;
    s32 use_offset;

    world_thread_set_current_task_id(NATIVE_THREAD_TASK_STATUS_PANEL);
    thread = (world_status_thread_t*)g_world_threads[g_world_thread_current_id].function_parameter_1;
    g_world_input_frame_controller_input = world_input_get_menu_controller(0) + 1;
    if (g_world_thread_current_id == NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) {
        buffer = g_world_editor_numeric_state_a;
        buffers = buffer;
        text_pixels = g_world_editor_numeric_text_a;
        upload_a = g_world_editor_numeric_entries_a;
        upload_b = g_world_editor_numeric_entries_b;
        upload_c = g_world_editor_numeric_entries_c;
        entries = g_world_editor_numeric_descriptor_a;
    } else {
        buffer = g_world_editor_numeric_state_b;
        buffers = buffer;
        text_pixels = g_world_editor_numeric_text_b;
        upload_a = g_world_editor_numeric_entries_d;
        upload_b = g_world_editor_numeric_entries_e;
        upload_c = g_world_editor_numeric_entries_f;
        entries = g_world_editor_numeric_descriptor_b;
    }
    world_gfx_set_draw_mode_for_texture_page(&buffer->draw_modes[0], MENU_TEXTURE_PAGE_FRAME);
    world_gfx_set_draw_mode_for_texture_page(&buffer->draw_modes[1], MENU_TEXTURE_PAGE_TEXT_IMAGES);
    world_gfx_set_draw_mode_for_texture_page(&buffer->draw_modes[2], MENU_TEXTURE_PAGE_ALLOCATED_IMAGES);
    world_menu_init_numeric_display_frame_primitives(&g_world_editor_numeric_table[0], &buffer->numeric_frame);
    world_gfx_init_scaled_draw_area_packets_3(&buffer->portrait);
    i = 0;
    descriptor = g_world_editor_numeric_texture;
    for (; i < 18; i++) {
        world_menu_init_sprite(&buffer->sprites[i]);
        world_gfx_init_image_loading(
            &buffer->sprites[i], g_world_editor_numeric_geometry, &g_world_editor_numeric_table[0], descriptor);
        descriptor++;
    }
    if (g_world_thread_current_id != NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) {
        buffer->sprites[14].v0 += 0x4C;
        buffer->sprites[15].v0 += 0x4C;
        buffer->sprites[16].v0 += 0x4C;
    }
    buffer->sprites[10].clut = 0x7D7C;
    world_script_copy_bytes(&buffer[1], buffer, sizeof(battle_menu_status_panel_numeric_buffer_t));

    for (frame = 0;; frame++) {
        buffer = &buffers[frame & 1];
        if (frame == 0 || thread->redraw_request != 0) {
            world_clear_menu_render_buffer(text_pixels, 0x918);
            g_world_menu_text_state.pen.stride = 0x14;
            world_menu_set_text_origin(0, 0);
            if (g_world_thread_current_id != NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) {
                world_text_render_signed_decimal_entries(text_pixels, entries, &g_world_menu_text_state.pen, 3);
            } else {
                world_menu_draw_numeric_display_entries(text_pixels, entries, &g_world_menu_text_state.pen, 3);
            }
            g_world_menu_text_state.pen.stride = 0x40;
            world_menu_set_text_origin(0, 0);
            if (g_world_thread_current_id != NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) {
                world_text_render_signed_decimal_entries(
                    text_pixels + 0x168, entries + 3, &g_world_menu_text_state.pen, 4);
            } else {
                world_menu_draw_numeric_display_entries(
                    text_pixels + 0x168, entries + 3, &g_world_menu_text_state.pen, 4);
            }
            g_world_menu_text_state.pen.stride = 0x64;
            world_menu_set_text_origin(0, 0);
            if (g_world_thread_current_id != NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) {
                world_text_render_signed_decimal_entries(
                    text_pixels + 0x468, entries + 7, &g_world_menu_text_state.pen, 8);
            } else {
                world_menu_draw_numeric_display_entries(
                    text_pixels + 0x468, entries + 7, &g_world_menu_text_state.pen, 8);
            }
            LoadImage(upload_a, (u32*)text_pixels);
            LoadImage(upload_b, (u32*)(text_pixels + 0x168));
            LoadImage(upload_c, (u32*)(text_pixels + 0x468));
        }
        field_y = g_world_frame_arg;
        use_offset = field_y != 0xF0;
        g_world_gfx_draw_area_y = -use_offset & 0xF0;
        world_gfx_build_scaled_draw_area_packets(
            &buffer->portrait, &g_world_editor_numeric_table[1], frame, field_y, thread);
        buffer->draw_offsets[0].x = thread->x - 0x80;
        buffer->draw_offsets[0].y = thread->y + g_world_gfx_draw_area_y;
        SetDrawOffset(&buffer->draw_offsets[0], &buffer->draw_offsets[0].x);
        buffer->draw_offsets[1].x = -0x80;
        buffer->draw_offsets[1].y = g_world_gfx_draw_area_y;
        SetDrawOffset(&buffer->draw_offsets[1], &buffer->draw_offsets[1].x);
        if (thread->style == 1 || g_world_thread_task_active == 1) {
            for (i = 17; i >= 0; i--) {
                buffer->sprites[i].clut = 0x7D3C;
            }
            buffer->sprites[10].clut = 0x7EBC;
            buffer->sprites[14].clut = 0x7F7D;
            buffer->sprites[15].clut = 0x7F7D;
            buffer->sprites[16].clut = 0x7F7D;
        } else {
            for (i = 17; i >= 0; i--) {
                buffer->sprites[i].clut = 0x7C3C;
            }
            buffer->sprites[10].clut = 0x7D7C;
            buffer->sprites[14].clut = 0x7FFC;
            buffer->sprites[15].clut = 0x7FFC;
            buffer->sprites[16].clut = 0x7FFC;
        }
        world_menu_select_primitive_color_palette(&buffer->numeric_frame, thread);
        if (frame < 13) {
            world_gfx_draw_or_append_gpu_primitive(&buffer->portrait.areas[1]);
        }
        world_gfx_draw_or_append_gpu_primitive(&buffer->draw_offsets[1]);
        for (i = 0; i < 14; i++) {
            world_gfx_draw_or_append_gpu_primitive(&buffer->sprites[i]);
        }
        world_gfx_draw_or_append_gpu_primitive(&buffer->sprites[17]);
        world_gfx_draw_or_append_gpu_primitive(&buffer->draw_modes[0]);
        for (; i < 17; i++) {
            world_gfx_draw_or_append_gpu_primitive(&buffer->sprites[i]);
        }
        if (g_world_thread_current_id == NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) {
            world_gfx_draw_or_append_gpu_primitive(&buffer->draw_modes[1]);
        } else {
            world_gfx_draw_or_append_gpu_primitive(&buffer->draw_modes[2]);
        }
        world_menu_submit_numeric_display_frame_primitives(&buffer->numeric_frame);
        world_gfx_draw_or_append_gpu_primitive(&buffer->draw_offsets[0]);
        if (frame < 13) {
            world_gfx_draw_or_append_gpu_primitive(&buffer->portrait);
        }
        world_thread_yield();
        if (world_thread_get_current_parameter_3() != 0) {
            break;
        }
    }
    world_thread_yield();
    world_thread_exit_current();
}
