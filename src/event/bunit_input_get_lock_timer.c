#include "fft/event_bunit.h"
#include "psx/types.h"

/* Return the input lock timer (g_bunit_input_lock_timer), nonzero while bunit_input_update_lock_timer
 * holds the menu input masks at zero. */
s32 bunit_input_get_lock_timer(void) {
    return g_bunit_input_lock_timer;
}
