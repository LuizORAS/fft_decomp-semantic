#include "fft/world.h"
#include "psx/pad.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_confirm_action_silently. */
void world_menu_confirm_action_silently(world_menu_entry_t* entry) {
    g_world_sound_suppressed = 1;
    g_world_menu_new_button_input = PSX_PAD_CIRCLE;
    world_menu_handle_entry_confirm(entry, 0);
    g_world_sound_suppressed = 0;
    g_world_menu_new_button_input = 0;
}
