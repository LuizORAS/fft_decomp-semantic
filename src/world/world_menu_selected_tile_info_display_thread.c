#include "fft/gfx.h"
#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Display thread at 0x800e8ab0: draws the selected tile's quad group
 * (the selected tile's display height, built by world_menu_build_quad_pieces) every frame and, when
 * the tile depth is non-zero, renders it as a one-digit
 * number into the icon sprite.  Runs until the third thread parameter is set. */
void world_menu_selected_tile_info_display_thread(void) {
    RECT rect;
    u8 buffer[0x100];
    s32 i;
    s32 j;
    s32 frame;
    menu_quad_page_t* page;
    POLY_GT4* quad;

    g_world_menu_hide_numeric_values = 0;
    page = &g_world_menu_selected_tile_info_pages[0];
    world_gfx_set_draw_mode_for_texture_page(&page->modes[0], MENU_TEXTURE_PAGE_ALLOCATED_IMAGES);
    world_gfx_set_draw_mode_for_texture_page(&page->modes[1], MENU_TEXTURE_PAGE_FRAME);
    for (i = 0; i < 5; i++) {
        quad = &page->quads[i];
        SetPolyGT4(quad);
        SetShadeTex(quad, 0);
        SetSemiTrans(quad, 1);
        quad->tpage = GetTPage(0, 0, 0x380, 0x120);
        quad->clut = 0x7CFC;
    }
    world_menu_init_sprite_array(page->sprites, 2, 0x7CBC);
    page->sprites[1].clut = 0x7D7C;
    for (i = 0; i < 2; i++) {
        gfx_init_image_loading(&page->sprites[i], &g_world_gfx_menu_image_source, &g_world_tile_info_image_location,
            &g_world_tile_info_image_params[i]);
    }
    world_text_configure_sprite_vram(&rect, 0x18, 0x10, &page->sprites[0], 0);
    world_script_copy_bytes(
        &g_world_menu_selected_tile_info_pages[1], &g_world_menu_selected_tile_info_pages[0], sizeof(menu_quad_page_t));

    for (frame = 0;; frame++) {
        map_selected_tile_t* tile = &g_world_map_selected_tile_data;
        s16* value = &tile->depth;

        page = &g_world_menu_selected_tile_info_pages[frame & 1];
        i = world_menu_build_quad_pieces(tile->display_height, page->quads) - 1;
        world_thread_yield();
        if (world_thread_find_running_by_task(NATIVE_THREAD_TASK_DARK_SCREEN) != 0) {
            continue;
        }
        if (world_thread_find_running_by_task(NATIVE_THREAD_TASK_DARK_SCREEN_HOLD) != 0) {
            continue;
        }
        g_world_menu_hide_numeric_values = 0;
        if (world_thread_get_current_parameter_3() != 0) {
            break;
        }
        for (j = i; j >= 0; j--) {
            world_gfx_draw_or_append_gpu_primitive(&page->quads[j]);
        }
        if (*value != 0) {
            world_clear_menu_render_buffer(buffer, 0x100);
            g_world_menu_text_state.pen.stride = 0x18;
            world_menu_set_text_origin(0x10, 0);
            world_menu_draw_number_glyphs(*value, 1, buffer, &g_world_menu_text_state.pen);
            if (g_world_gfx_draw_suppress_flag == 0) {
                LoadImage(&rect, buffer);
                page->sprites[0].clut = 0x7CBC;
                world_gfx_draw_or_append_gpu_primitive(&page->sprites[0]);
                world_gfx_draw_or_append_gpu_primitive(&page->modes[0]);
                world_gfx_draw_or_append_gpu_primitive(&page->sprites[1]);
                world_gfx_draw_or_append_gpu_primitive(&page->modes[1]);
            }
        }
    }
    world_thread_yield();
    world_gfx_free_tpage7_vram(&rect);
    world_thread_exit_current();
}
