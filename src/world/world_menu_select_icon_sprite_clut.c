#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_select_icon_sprite_clut. */
void world_menu_select_icon_sprite_clut(SPRT* slot) {
    s32 icon;

    if (world_thread_is_previous_running() != 0 || g_menu_input_lock_mode == 1) {
        icon = 0x7C7C;
    } else {
        icon = 0x7CBC;
    }
    slot->clut = icon;
}
