#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Keep the input lockout timer of the formation and shop screens (g_world_input_lockout_timer): 10
 * while the comparison banner (thread 7) runs or the status banner slides, otherwise one less each
 * frame down to 0. While it is nonzero the frame's pad state is cleared (world_input_clear_state). */
void world_input_update_lockout_timer(void) {
    s32 value;

    if (thread_is_running(THREAD_SCREEN_SLOT_COMPARISON_STATUS) != 0) {
        value = 10;
    } else if (g_world_status_display_thread_params.flags
        & (MENU_STATUS_PANEL_SLIDE_DOWN | MENU_STATUS_PANEL_SLIDE_UP)) {
        value = 10;
    } else {
        if (g_world_input_lockout_timer == 0) {
            return;
        }
        value = g_world_input_lockout_timer - 1;
    }
    g_world_input_lockout_timer = value;
    if (g_world_input_lockout_timer != 0) {
        world_input_clear_state();
    }
}
