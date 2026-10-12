#include "fft/world.h"
#include "psx/libgpu.h"

/* Select the WORLD image texture page and initialize its draw mode. */
void world_gfx_set_draw_mode_for_texture_page(DR_MODE* mode, s32 image_type) {
    if (image_type == MENU_TEXTURE_PAGE_FRAME) {
        SetDrawMode(mode, 0, 0, GetTPage(0, 0, 960, 256), &g_world_menu_disabled_texture_window);
    } else if (image_type == MENU_TEXTURE_PAGE_ALLOCATED_IMAGES) {
        SetDrawMode(mode, 0, 0, GetTPage(0, 0, 448, 0), &g_world_menu_disabled_texture_window);
    } else if (image_type == MENU_TEXTURE_PAGE_TEXT_IMAGES) {
        SetDrawMode(mode, 0, 0, GetTPage(0, 0, 384, 0), &g_world_menu_disabled_texture_window);
    } else if (image_type == MENU_TEXTURE_PAGE_UNIT_SPRITES) {
        SetDrawMode(mode, 0, 0, GetTPage(0, 0, 832, 256), &g_world_menu_disabled_texture_window);
    } else {
        SetDrawMode(mode, 0, 0, GetTPage(0, 0, 896, 288), &g_world_menu_disabled_texture_window);
    }
}
