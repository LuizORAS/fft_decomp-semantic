#include "fft/battle_gfx.h"
#include "fft/battle_menu.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/*
 * Set a BATTLE draw mode from one of five fixed texture-page origins.
 *
 * The repeated calls are what the target emits: GCC cross-jumps the five
 * identical tails into the single shared GetTPage/SetDrawMode sequence, so
 * hoisting the call out of the branches produces different code.
 */
void battle_gfx_set_draw_mode_for_texture_page(DR_MODE* mode, s32 page) {
    if (page == MENU_TEXTURE_PAGE_FRAME) {
        SetDrawMode(mode, 0, 0, GetTPage(0, 0, 0x3c0, 0x100), &g_battle_menu_disabled_texture_window);
    } else if (page == MENU_TEXTURE_PAGE_ALLOCATED_IMAGES) {
        SetDrawMode(mode, 0, 0, GetTPage(0, 0, 0x1c0, 0), &g_battle_menu_disabled_texture_window);
    } else if (page == MENU_TEXTURE_PAGE_TEXT_IMAGES) {
        SetDrawMode(mode, 0, 0, GetTPage(0, 0, 0x180, 0), &g_battle_menu_disabled_texture_window);
    } else if (page == MENU_TEXTURE_PAGE_UNIT_SPRITES) {
        SetDrawMode(mode, 0, 0, GetTPage(0, 0, 0x340, 0x100), &g_battle_menu_disabled_texture_window);
    } else {
        SetDrawMode(mode, 0, 0, GetTPage(0, 0, 0x380, 0x120), &g_battle_menu_disabled_texture_window);
    }
}
