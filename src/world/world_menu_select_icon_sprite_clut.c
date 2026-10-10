#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Select the CLUT of a menu icon sprite: 0x7C7C while the previous thread
 * slot is running or a text task is active, otherwise 0x7CBC. */
void world_menu_select_icon_sprite_clut(SPRT* slot) {
    s32 icon;

    if (world_thread_is_previous_running() != 0 || g_world_thread_task_active == 1) {
        icon = 0x7C7C;
    } else {
        icon = 0x7CBC;
    }
    slot->clut = icon;
}
