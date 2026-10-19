#include "fft/battle.h"

/* Clear g_main_system_min_vsync_interval, the minimum frame wait
 * battle_state_set_min_vsync_interval sets; the effect stage calls it. */
void battle_state_clear_min_vsync_interval(void) {
    g_main_system_min_vsync_interval = 0;
}
