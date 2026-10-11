#include "fft/event_equip.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* The primitive block is battle_menu_status_panel_numeric_buffer_t, double buffered
 * (g_equip_editor_numeric_state_a for thread 12, g_equip_editor_numeric_state_b otherwise); the thread parameter
 * block is world_status_thread_t. */

/* Thread task 0x3b: numeric editor panel.
 *
 * Twin of attack_editor_run_numeric_thread / bunit_menu_run_numeric_editor_thread
 * with EQUIP's own draw-mode setup. Builds the primitive block once, copies it
 * into the second buffer, then each frame re-renders the three text images when
 * requested, rescales the draw area, recolours the sprites for the thread
 * style and submits the block until the thread's third parameter is set.
 * `buffers` is assigned in each branch (not after the if) so the spilled store
 * precedes upload_c's at the join, as in the target. */
void equip_editor_run_numeric_thread(void) {
    world_status_thread_t* thread;
    battle_menu_status_panel_numeric_buffer_t* buffer;
    battle_menu_status_panel_numeric_buffer_t* buffers;
    u8* text_pixels;
    menu_number_entry_t* entries;
    RECT* upload_a;
    RECT* upload_b;
    RECT* upload_c;
    const world_gfx_image_load_parameters_t* descriptor;
    s32 i;
    s32 frame;
    s32 field_y;
    s32 use_offset;

    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_STATUS_PANEL);
    thread = (world_status_thread_t*)g_battle_threads[g_battle_current_thread_id].function_parameter_1;
    g_equip_input_controller = battle_script_get_controller_input_pointer(0) + 1;
    if (g_battle_current_thread_id == NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) {
        buffer = g_equip_editor_numeric_state_a;
        buffers = buffer;
        text_pixels = g_equip_editor_numeric_text_a;
        upload_a = &g_equip_editor_numeric_entries_a;
        upload_b = &g_equip_editor_numeric_entries_b;
        upload_c = &g_equip_editor_numeric_entries_c;
        entries = g_equip_editor_numeric_descriptor_a;
    } else {
        buffer = g_equip_editor_numeric_state_b;
        buffers = buffer;
        text_pixels = g_equip_editor_numeric_text_b;
        upload_a = &g_equip_editor_numeric_entries_d;
        upload_b = &g_equip_editor_numeric_entries_e;
        upload_c = &g_equip_editor_numeric_entries_f;
        entries = g_equip_editor_numeric_descriptor_b;
    }
    battle_gfx_set_draw_mode_for_texture_page(&buffer->draw_modes[0], MENU_TEXTURE_PAGE_FRAME);
    battle_gfx_set_draw_mode_for_texture_page(&buffer->draw_modes[1], MENU_TEXTURE_PAGE_TEXT_IMAGES);
    SetDrawMode(&buffer->draw_modes[2], 0, 0, GetTPage(0, 2, 0x140, 0), &g_equip_gfx_draw_area_template);
    battle_menu_init_numeric_display_frame_primitives(
        &g_equip_editor_numeric_table.texture_rect, &buffer->numeric_frame);
    equip_gfx_init_scaled_draw_area_packets(&buffer->portrait);
    i = 0;
    descriptor = g_equip_editor_numeric_texture;
    for (; i < 18; i++) {
        battle_menu_init_semitransparent_sprt(&buffer->sprites[i]);
        battle_gfx_init_image_loading(&buffer->sprites[i], g_equip_editor_numeric_geometry,
            &g_equip_editor_numeric_table.texture_rect, descriptor);
        descriptor++;
    }
    if (g_battle_current_thread_id != NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) {
        buffer->sprites[14].v0 = 0x6C;
        buffer->sprites[15].v0 = 0x6C;
        buffer->sprites[16].v0 = 0x6C;
    }
    buffer->sprites[10].clut = 0x7D7C;
    battle_copy_bytes(&buffer[1], buffer, sizeof(battle_menu_status_panel_numeric_buffer_t));

    for (frame = 0;; frame++) {
        buffer = &buffers[frame & 1];
        if (frame == 0 || thread->redraw_request != 0) {
            battle_clear_menu_render_buffer(text_pixels, 0x918);
            g_menu_text_state.pen.stride = 0x14;
            battle_menu_set_text_origin(0, 0);
            if (g_battle_current_thread_id != NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) {
                equip_text_render_signed_decimal_entries(text_pixels, entries, &g_menu_text_state.pen, 3);
            } else {
                battle_menu_draw_numeric_display_entries(text_pixels, entries, &g_menu_text_state.pen, 3);
            }
            g_menu_text_state.pen.stride = 0x40;
            battle_menu_set_text_origin(0, 0);
            if (g_battle_current_thread_id != NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) {
                equip_text_render_signed_decimal_entries(text_pixels + 0x168, entries + 3, &g_menu_text_state.pen, 4);
            } else {
                battle_menu_draw_numeric_display_entries(text_pixels + 0x168, entries + 3, &g_menu_text_state.pen, 4);
            }
            g_menu_text_state.pen.stride = 0x64;
            battle_menu_set_text_origin(0, 0);
            if (g_battle_current_thread_id != NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) {
                equip_text_render_signed_decimal_entries(text_pixels + 0x468, entries + 7, &g_menu_text_state.pen, 8);
            } else {
                battle_menu_draw_numeric_display_entries(text_pixels + 0x468, entries + 7, &g_menu_text_state.pen, 8);
            }
            LoadImage(upload_a, (u32*)text_pixels);
            LoadImage(upload_b, (u32*)(text_pixels + 0x168));
            LoadImage(upload_c, (u32*)(text_pixels + 0x468));
        }
        field_y = g_main_gfx_screen_polarity * 0xF0;
        use_offset = field_y != 0xF0;
        g_equip_gfx_draw_offset_y = -use_offset & 0xF0;
        equip_gfx_build_scaled_draw_area_packets(
            &buffer->portrait, &g_equip_editor_numeric_table.draw_area_rect, frame, field_y, thread);
        buffer->draw_offsets[0].position.x = thread->x - 0x80;
        buffer->draw_offsets[0].position.y = thread->y + g_equip_gfx_draw_offset_y;
        SetDrawOffset(&buffer->draw_offsets[0].packet, (u16*)&buffer->draw_offsets[0].position);
        buffer->draw_offsets[1].position.x = -0x80;
        buffer->draw_offsets[1].position.y = g_equip_gfx_draw_offset_y;
        SetDrawOffset(&buffer->draw_offsets[1].packet, (u16*)&buffer->draw_offsets[1].position);
        if (thread->style == 1 || g_battle_menu_input_lock_mode == 1) {
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
        equip_menu_select_primitive_color_palette(&buffer->numeric_frame, thread);
        if (frame < 13) {
            battle_gfx_draw_or_append_gpu_primitive(&buffer->portrait.areas[1]);
        }
        battle_gfx_draw_or_append_gpu_primitive(&buffer->draw_offsets[1]);
        for (i = 0; i < 14; i++) {
            battle_gfx_draw_or_append_gpu_primitive(&buffer->sprites[i]);
        }
        battle_gfx_draw_or_append_gpu_primitive(&buffer->sprites[17]);
        battle_gfx_draw_or_append_gpu_primitive(&buffer->draw_modes[0]);
        for (; i < 17; i++) {
            battle_gfx_draw_or_append_gpu_primitive(&buffer->sprites[i]);
        }
        if (g_battle_current_thread_id == NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) {
            battle_gfx_draw_or_append_gpu_primitive(&buffer->draw_modes[1]);
        } else {
            battle_gfx_draw_or_append_gpu_primitive(&buffer->draw_modes[2]);
        }
        battle_menu_submit_numeric_display_frame_primitives(&buffer->numeric_frame);
        battle_gfx_draw_or_append_gpu_primitive(&buffer->draw_offsets[0]);
        if (frame < 13) {
            battle_gfx_draw_or_append_gpu_primitive(&buffer->portrait);
        }
        battle_thread_yield();
        if (battle_thread_get_current_parameter_3() != 0) {
            break;
        }
    }
    battle_thread_yield();
    battle_thread_exit_current();
}
