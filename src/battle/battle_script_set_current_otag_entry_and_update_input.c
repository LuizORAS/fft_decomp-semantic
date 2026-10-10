#include "fft/battle.h"
#include "psx/types.h"

/* Update the event input state from `input`, make `entry` the active ordering
 * table and draw the open window frames. */
void battle_script_set_current_otag_entry_and_update_input(void* entry, s32 input) {
    battle_script_update_event_input_state(input);
    g_battle_gfx_current_otag_entry = entry;
    battle_menu_draw_active_window_frames();
}
