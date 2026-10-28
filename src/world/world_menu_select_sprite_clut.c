#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_select_sprite_clut: the dimmed CLUT (0x7D3C) while the previous thread
 * slot runs or g_menu_input_lock_mode is 1, otherwise the normal one (0x7C3C). */
void world_menu_select_sprite_clut(void* packet) {
    s32 icon;
    world_texture_primitive_t* primitive = packet;

    if (world_thread_is_previous_running() != 0 || g_menu_input_lock_mode == 1) {
        icon = 0x7D3C;
    } else {
        icon = 0x7C3C;
    }
    primitive->sprt.clut = icon;
}
