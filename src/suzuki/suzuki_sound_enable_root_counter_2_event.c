#include "psx/libetc.h"
#include "psx/suzuki.h"
#include "psx/types.h"

void suzuki_sound_enable_root_counter_2_event(void) {
    if ((g_suzuki_sound_driver_flags & SUZUKI_SOUND_DRIVER_ROOT_COUNTER_EVENT) == 0) {
        g_suzuki_sound_driver_flags |= SUZUKI_SOUND_DRIVER_ROOT_COUNTER_EVENT;
        EnableEvent(g_suzuki_sound_root_counter_2_event);
    }
}
