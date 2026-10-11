#include "fft/world.h"

/* One pointer holds each loaded resource in turn. */
typedef union world_menu_graphics_resource {
    world_menu_image_pages_t pages;
    main_gfx_frame_bin_t frame;
} world_menu_graphics_resource_t;

/*
 * Load the common menu images, palettes, and optional extra image pages.
 *
 * The first resource is cached across calls. Each temporary allocation stays
 * alive until its queued VRAM transfers finish.
 */
void world_menu_load_common_graphics(s32 load_extra_pages) {
    world_menu_graphics_resource_t* resource;

    if (g_main_common_menu_graphics_loaded == 0) {
        g_world_thread_call_target = main_file_load_checked_to_heap;
        resource = (world_menu_graphics_resource_t*)world_thread_call_on_main_stack(DISC_SECTOR_EVENT_ITEM_BIN, 0x8800);
        LoadImage(&g_world_menu_item_image_rect, resource->pages.first_page);
        LoadImage(&g_world_menu_item_palette_rect, resource->pages.second_page);
        DrawSync(0);
        main_heap_call_free(resource);
        g_main_common_menu_graphics_loaded++;
    }
    /* The SDK transfers packed colors as words. */
    LoadImage(&g_world_menu_common_clut_rect, (u32*)&g_world_menu_common_clut_data);
    DrawSync(0);
    g_world_thread_call_target = main_file_load_checked_to_heap;
    resource = (world_menu_graphics_resource_t*)world_thread_call_on_main_stack(
        DISC_SECTOR_EVENT_FRAME_BIN, sizeof(main_gfx_frame_bin_t));
    LoadImage(&g_world_menu_frame_image_rect, (u32*)resource->frame.pixels[32]);
    LoadImage(&g_world_menu_frame_palette_rect, (u32*)resource->frame.clut);
    LoadImage(&g_world_menu_frame_palette_tail_rect, (u32*)resource->frame.palette_tail);
    world_menu_set_palette_colors(&g_world_menu_glyph_sheet->palette);
    DrawSync(0);
    main_heap_call_free(resource);
    if (load_extra_pages != 0) {
        g_world_thread_call_target = main_file_load_checked_to_heap;
        resource
            = (world_menu_graphics_resource_t*)world_thread_call_on_main_stack(DISC_SECTOR_EVENT_UNIT_BIN, 0x10000);
        LoadImage(&g_world_menu_unit_image_rect_a, resource->pages.first_page);
        LoadImage(&g_world_menu_unit_image_rect_b, resource->pages.second_page);
        DrawSync(0);
        main_heap_call_free(resource);
    }
}
