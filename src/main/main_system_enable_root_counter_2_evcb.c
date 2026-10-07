#include "fft/main.h"
#include "psx/libetc.h"
#include "psx/types.h"

/* Enable the root counter 2 event, the one main_smd_* functions mask while they change driver
 * state, unless bit 0 of g_main_sound_driver_flags says it is already on. */
void main_system_enable_root_counter_2_evcb(void) {
    u16 flags;

    flags = g_main_sound_driver_flags;
    if ((flags & 1) == 0) {
        g_main_sound_driver_flags = flags | 1;
        EnableEvent(g_main_root_counter_2_event);
    }
}
