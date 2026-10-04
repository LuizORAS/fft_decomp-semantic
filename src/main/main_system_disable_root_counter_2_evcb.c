#include "fft/main.h"
#include "psx/libapi.h"
#include "psx/types.h"

/* Disable the root counter 2 event if bit 0 of g_main_sound_driver_flags says it is on, and clear
 * the bit. */
void main_system_disable_root_counter_2_evcb(void) {
    if ((g_main_sound_driver_flags & 1) != 0) {
        DisableEvent(g_main_root_counter_2_event);
        g_main_sound_driver_flags &= ~1;
    }
}
