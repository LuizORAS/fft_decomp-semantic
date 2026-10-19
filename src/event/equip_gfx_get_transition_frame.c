#include "fft/event_equip.h"
#include "psx/types.h"

/* Return the window-opening step (g_equip_gfx_transition_frame): MENU_SCRIPT_OP_OPENING_WINDOW_FRAME
 * clips a window by it until it reaches 4, and MENU_SCRIPT_OP_ADVANCE_WINDOW_SCALE advances it. */
s32 equip_gfx_get_transition_frame(void) {
    return g_equip_gfx_transition_frame;
}
