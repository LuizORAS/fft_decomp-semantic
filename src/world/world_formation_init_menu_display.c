#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Set up the display of WORLD's menu screens (the formation, the shops, name entry and the card
 * screen): reset the menu context, clear the screen unless g_world_shop_id is
 * WORLD_SHOP_ID_NAME_ENTRY, upload the menu glyph sheet, the formation palettes and two 16-colour CLUTs
 * copied from VRAM rows 0x1f0/0x1f4 with entries 4, 8 and 12 made transparent, then reset the
 * description text, list, cursors and menu results, start the fade-in, bind the skillset, job,
 * ability and formation message texts and point the composed and screen text sections at the
 * formation menu text.
 *
 * Compiled with the -O1 profile: -O2 scheduling reorders the RECT stores. */
void world_formation_init_menu_display(void) {
    RECT rect;
    u16 clut[16];

    world_menu_reset_context();
    world_script_set_vsync_mode_and_event_speed(0);
    if (g_world_shop_id != WORLD_SHOP_ID_NAME_ENTRY) {
        rect.x = 0;
        rect.y = 0;
        rect.w = 0x100;
        rect.h = 0x1E0;
        ClearImage(&rect, 0, 0, 0);
        SetDispMask(1);
    }
    rect.x = 0x240;
    rect.y = 0x100;
    rect.w = 0x40;
    rect.h = 0x30;
    world_gfx_load_image_and_wait(&rect, g_world_menu_glyph_image);
    rect.x = 0x240;
    rect.y = 0x1FA;
    rect.w = 0x40;
    rect.h = 4;
    world_gfx_load_image_and_wait(&rect, g_world_formation_menu_palettes);
    rect.x = 0x3C0;
    rect.y = 0x1F0;
    rect.w = 0x10;
    rect.h = 1;
    world_gfx_store_image_and_wait(&rect, clut);
    clut[4] = 0;
    clut[8] = 0;
    clut[12] = 0;
    rect.x = 0x260;
    rect.y = 0x1FE;
    rect.w = 0x10;
    rect.h = 1;
    world_gfx_load_image_and_wait(&rect, clut);
    rect.x = 0x240;
    rect.y = 0x1FE;
    rect.w = 0xC;
    rect.h = 1;
    world_gfx_load_image_and_wait(&rect, &clut[4]);
    rect.x = 0x3C0;
    rect.y = 0x1F4;
    rect.w = 0x10;
    rect.h = 1;
    world_gfx_store_image_and_wait(&rect, clut);
    clut[4] = 0;
    clut[8] = 0;
    clut[12] = 0;
    rect.x = 0x270;
    rect.y = 0x1FE;
    rect.w = 0x10;
    rect.h = 1;
    world_gfx_load_image_and_wait(&rect, clut);
    rect.x = 0x250;
    rect.y = 0x1FE;
    rect.w = 0xC;
    rect.h = 1;
    world_gfx_load_image_and_wait(&rect, &clut[4]);
    world_gfx_load_image_and_wait(&g_world_formation_status_label_image_rect, g_world_formation_status_label_image);
    g_world_menu_description_text_id = 0;
    world_menu_init_scrollable_list_core(0, 0, 0);
    world_gfx_start_decreasing_fade();
    world_menu_clear_cursor_positions();
    g_world_formation_unit_cycle_mode = 0;
    world_menu_reset_selection_results();
    world_text_bind_section_data(WORLD_TEXT_DATA_SKILLSET_NAMES);
    world_text_bind_section_data(WORLD_TEXT_DATA_JOB_NAMES);
    world_text_bind_section_data(WORLD_TEXT_DATA_ABILITY_NAMES);
    world_text_bind_section_data(WORLD_TEXT_DATA_FORMATION_MESSAGES);
    g_world_text_section_pointers[TEXT_SECTION_COMPOSED_TEXT] = g_world_formation_menu_text_buffer;
    g_world_text_section_pointers[TEXT_SECTION_OVERLAY_SCREEN] = g_world_formation_menu_text_table;
    g_world_formation_cursor_ot_override = 0;
    g_world_tutorial_id = 0;
}
