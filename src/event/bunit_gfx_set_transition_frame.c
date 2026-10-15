#include "fft/event_bunit.h"
#include "psx/types.h"

/* Set the window-opening step (g_bunit_gfx_transition_frame); the screens set 10 to show their windows at full size at
 * once. */
void bunit_gfx_set_transition_frame(s32 frame) {
    g_bunit_gfx_transition_frame = frame;
}
