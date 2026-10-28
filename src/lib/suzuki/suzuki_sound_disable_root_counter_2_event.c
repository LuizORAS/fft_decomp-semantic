#include "psx/libapi.h"
#include "psx/suzuki.h"

/* Disable the root counter 2 event if bit 0 of g_suzuki_sound_driver_flags says it is on, and clear
 * the bit. */
void suzuki_sound_disable_root_counter_2_event(void) {
    if ((g_suzuki_sound_driver_flags & MAIN_SUZUKI_SOUND_DRIVER_ROOT_COUNTER_EVENT) != 0) {
        DisableEvent(g_suzuki_sound_root_counter_2_event);
        g_suzuki_sound_driver_flags &= ~MAIN_SUZUKI_SOUND_DRIVER_ROOT_COUNTER_EVENT;
    }
}
