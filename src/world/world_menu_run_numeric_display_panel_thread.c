#include "fft/gfx.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Thread task 0x23: renders three text images into VRAM, builds two
 * alternating 18-sprite panel records with line boxes, then redraws the panel
 * every frame (dimmed palette while thread 6 runs) until the thread's
 * third parameter becomes non-zero. */
void world_menu_run_numeric_display_panel_thread(void) {
    menu_panel_record_t* records;
    menu_panel_record_t* record;
    menu_unit_stat_number_images_t* number_images;
    s32 i;
    s32 frame;

    record = g_world_numeric_display_panel_records;
    records = record;
    thread_set_current_task_id(NATIVE_THREAD_TASK_NUMERIC_DISPLAY_PANEL);
    world_gfx_set_draw_mode_for_texture_page(&record->mode0, MENU_TEXTURE_PAGE_FRAME);
    world_gfx_set_draw_mode_for_texture_page(&record->mode1, MENU_TEXTURE_PAGE_TEXT_IMAGES);
    world_gfx_init_scaled_draw_area_packets(&record->draw_areas);
    number_images = world_menu_alloc_ui_buffer(sizeof(*number_images));
    world_clear_menu_render_buffer(number_images, sizeof(*number_images));
    g_world_menu_text_state.pen.stride = 0x14;
    world_menu_set_text_origin(0, 0);
    world_menu_draw_numeric_display_entries(
        number_images->mobility, g_world_numeric_display_entries, &g_world_menu_text_state.pen, 3);
    g_world_menu_text_state.pen.stride = 0x40;
    world_menu_set_text_origin(0, 0);
    world_menu_draw_numeric_display_entries(
        number_images->weapons, &g_world_numeric_display_entries[3], &g_world_menu_text_state.pen, 4);
    g_world_menu_text_state.pen.stride = 0x64;
    world_menu_set_text_origin(0, 0);
    world_menu_draw_numeric_display_entries(
        number_images->combat, &g_world_numeric_display_entries[7], &g_world_menu_text_state.pen, 8);
    LoadImage(&g_world_numeric_display_text_rect_a, (u32*)number_images->mobility);
    LoadImage(&g_world_numeric_display_text_rect_b, (u32*)number_images->weapons);
    LoadImage(&g_world_numeric_display_text_rect_c, (u32*)number_images->combat);
    thread_wait_frames(1);
    world_menu_free_memory(number_images);
    for (i = 0; i < 18; i++) {
        world_menu_init_sprite(&record->sprites[i]);
        gfx_init_image_loading(&record->sprites[i], &g_world_gfx_menu_image_source, &g_world_numeric_display_frame_rect,
            &g_world_numeric_display_image_params[i]);
    }
    world_script_copy_bytes(&record[1], record, sizeof(record[1]));
    record[0].palette = &g_world_numeric_display_palettes[0].primitives;
    record[1].palette = &g_world_numeric_display_palettes[1].primitives;
    world_menu_init_numeric_display_frame_primitives(&g_world_numeric_display_frame_rect, record[0].palette);
    world_menu_init_numeric_display_frame_primitives(&g_world_numeric_display_frame_rect, record[1].palette);
    for (frame = 0;; frame++) {
        thread_yield();
        record = &records[frame & 1];
        world_gfx_build_scaled_draw_area_pair_swapped(
            &record->draw_areas, &g_world_numeric_display_texture_window, frame, g_main_gfx_screen_polarity);
        if (world_thread_get_current_parameter_3() != 0) {
            break;
        }
        if (world_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU_GRANDCHILD) != 0) {
            for (i = 17; i >= 0; i--) {
                record->sprites[i].clut = 0x7D3C;
            }
            record->sprites[10].clut = 0x7EBC;
            world_menu_init_primitive_colors_dimmed(record->palette);
        } else {
            for (i = 17; i >= 0; i--) {
                record->sprites[i].clut = 0x7C3C;
            }
            record->sprites[10].clut = 0x7D7C;
            record->sprites[14].clut = 0x7FFC;
            record->sprites[15].clut = 0x7FFC;
            record->sprites[16].clut = 0x7FFC;
            world_menu_init_primitive_colors_normal(record->palette);
        }
        world_gfx_draw_or_append_gpu_primitive(&record->draw_areas.areas[1]);
        for (i = 0; i < 14; i++) {
            world_gfx_draw_or_append_gpu_primitive(&record->sprites[i]);
        }
        world_gfx_draw_or_append_gpu_primitive(&record->sprites[17]);
        world_gfx_draw_or_append_gpu_primitive(&record->mode0);
        for (; i < 17; i++) {
            world_gfx_draw_or_append_gpu_primitive(&record->sprites[i]);
        }
        world_gfx_draw_or_append_gpu_primitive(&record->mode1);
        world_menu_submit_numeric_display_frame_primitives(record->palette);
        world_gfx_draw_or_append_gpu_primitive(&record->draw_areas.areas[0]);
    }
    thread_yield();
    thread_exit_current();
}
