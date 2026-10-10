#include "fft/world.h"
#include "psx/types.h"

/* Updates the menu repeat input from `buttons`, makes `ot` the active otag list and draws the open
 * window frames. */
void world_script_set_current_otag_entry_and_update_input(u32* ot, s32 buttons) {
    world_input_update_menu_repeat(buttons);
    g_world_gfx_active_otag_entries = ot;
    world_menu_draw_active_window_frames();
}
