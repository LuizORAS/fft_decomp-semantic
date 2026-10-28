#include "psx/suzuki.h"

/* Enable the root counter 2 event, the one suzuki_smd_* functions mask while they change driver
 * state, unless bit 0 of g_suzuki_sound_driver_flags says it is already on. */
void suzuki_sound_enable_root_counter_2_event(void) {
    if ((g_suzuki_sound_driver_flags & MAIN_SUZUKI_SOUND_DRIVER_ROOT_COUNTER_EVENT) == 0) {
        g_suzuki_sound_driver_flags |= MAIN_SUZUKI_SOUND_DRIVER_ROOT_COUNTER_EVENT;
        EnableEvent(g_suzuki_sound_root_counter_2_event);
    }
}
