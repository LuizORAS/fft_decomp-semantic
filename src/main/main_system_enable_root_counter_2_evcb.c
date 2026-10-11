#include "fft/main.h"
#include "psx/libetc.h"
#include "psx/types.h"

void main_system_enable_root_counter_2_evcb(void) {
    if ((g_main_sound_driver_flags & MAIN_SOUND_DRIVER_ROOT_COUNTER_EVENT) == 0) {
        g_main_sound_driver_flags |= MAIN_SOUND_DRIVER_ROOT_COUNTER_EVENT;
        EnableEvent(g_main_root_counter_2_event);
    }
}
