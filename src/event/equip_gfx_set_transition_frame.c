#include "fft/event_equip.h"
#include "psx/types.h"

/* Set the window-opening step (g_equip_gfx_transition_frame); the screens set 10 to show their windows at full size at
 * once. */
void equip_gfx_set_transition_frame(s32 value) {
    g_equip_gfx_transition_frame = value;
}
