#include "fft/battle.h"
#include "psx/libgpu.h"

/* Select the menu-sprite palette row for the active thread context. */
void battle_menu_select_icon_sprite_clut(SPRT* sprite) {
    s32 clut;

    if (battle_thread_is_previous_running() != 0 || g_battle_menu_input_lock_mode == 1) {
        clut = 0x7c7c;
    } else {
        clut = 0x7cbc;
    }
    sprite->clut = clut;
}
