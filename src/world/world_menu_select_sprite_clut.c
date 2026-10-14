#include "fft/world.h"
#include "psx/types.h"

/* Select the CLUT of a menu sprite or textured quad: the dimmed one (0x7D3C) while the
 * previous thread slot is running or a text task is active, otherwise the
 * normal one (0x7C3C). */
void world_menu_select_sprite_clut(void* packet) {
    s32 icon;
    texture_primitive_t* primitive = packet;

    if (world_thread_is_previous_running() != 0 || g_menu_input_lock_mode == 1) {
        icon = 0x7D3C;
    } else {
        icon = 0x7C3C;
    }
    primitive->sprt.clut = icon;
}
