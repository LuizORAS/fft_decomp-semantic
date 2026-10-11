#include "psx/libapi.h"
#include "psx/suzuki.h"
#include "psx/types.h"

void main_system_disable_root_counter_2_evcb(void) {
    if ((g_main_sound_driver_flags & MAIN_SOUND_DRIVER_ROOT_COUNTER_EVENT) != 0) {
        DisableEvent(g_main_root_counter_2_event);
        g_main_sound_driver_flags &= ~MAIN_SOUND_DRIVER_ROOT_COUNTER_EVENT;
    }
}
