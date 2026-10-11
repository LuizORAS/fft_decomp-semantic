#include "fft/world.h"
#include "psx/types.h"

/* Select the CLUT of a menu sprite: the dimmed one (0x7D3C) while the
 * previous thread slot is running or a text task is active, otherwise the
 * normal one (0x7C3C). */
void world_menu_select_sprite_clut(texture_primitive_t* primitive) {
    s32 icon;

    if (world_thread_is_previous_running() != 0 || g_world_thread_task_active == 1) {
        icon = 0x7D3C;
    } else {
        icon = 0x7C3C;
    }
    primitive->sprt.clut = icon;
}
