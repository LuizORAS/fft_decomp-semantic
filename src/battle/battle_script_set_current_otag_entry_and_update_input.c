#include "fft/battle_action.h"
#include "fft/battle_gfx.h"
#include "fft/battle_menu.h"
#include "fft/battle_script.h"
#include "fft/gfx.h"
#include "psx/types.h"

/* Update the event input state from `input`, make `entry` the active ordering
 * table and draw the open window frames. */
void battle_script_set_current_otag_entry_and_update_input(u32* entry, s32 input) {
    battle_script_update_event_input_state(input);
    g_gfx_current_otag_entry = entry;
    battle_menu_draw_active_window_frames();
}
