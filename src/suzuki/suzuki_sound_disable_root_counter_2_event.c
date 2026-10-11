#include "psx/libapi.h"
#include "psx/suzuki.h"
#include "psx/types.h"

void suzuki_sound_disable_root_counter_2_event(void) {
    if ((g_suzuki_sound_driver_flags & SUZUKI_SOUND_DRIVER_ROOT_COUNTER_EVENT) != 0) {
        DisableEvent(g_suzuki_sound_root_counter_2_event);
        g_suzuki_sound_driver_flags &= ~SUZUKI_SOUND_DRIVER_ROOT_COUNTER_EVENT;
    }
}
