#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_select_sprite_clut. */
void world_menu_select_sprite_clut(void* prim) {
    world_texture_prim_t* slot = prim;
    s32 icon;

    if (world_thread_is_previous_running() != 0 || g_world_menu_input_lock_mode == 1) {
        icon = 0x7D3C;
    } else {
        icon = 0x7C3C;
    }
    slot->sprt.clut = icon;
}
