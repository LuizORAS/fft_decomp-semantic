#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Return the window-opening step (g_jobstts_gfx_transition_frame), which every display script run
 * advances up to 10 and jobstts_menu_script_draw_opening_window_frame clips by until it reaches 8. */
s32 jobstts_gfx_get_transition_frame(void) {
    return g_jobstts_gfx_transition_frame;
}
