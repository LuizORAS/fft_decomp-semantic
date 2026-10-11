#include "fft/world.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Runs the menu's confirm handler on a simulated, silent Circle press with
 * option 0 selected, then clears the injected input. */
void world_menu_confirm_action_silently(world_menu_entry_t* entry) {
    g_world_menu_sound_muted = 1;
    g_world_menu_new_button_input = PSX_PAD_CIRCLE;
    world_menu_handle_entry_confirm(entry, 0);
    g_world_menu_sound_muted = 0;
    g_world_menu_new_button_input = 0;
}
