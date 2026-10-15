#include "fft/wldcore.h"
#include "psx/types.h"

/* Clear the auto-repeat counters of the D-pad, L1 and R1 and copy MAIN's repeat timing: the
 * initial delay, the period, and the shoulder buttons' period. */
void wldcore_input_init_repeat_state(void) {
    g_wldcore_input_repeat_counter_l1 = 0;
    g_wldcore_input_repeat_counter_r1 = 0;
    g_wldcore_input_repeat_counter_right = 0;
    g_wldcore_input_repeat_counter_left = 0;
    g_wldcore_input_repeat_counter_down = 0;
    g_wldcore_input_repeat_counter_up = 0;
    g_wldcore_input_repeat_initial_delay = g_main_input_repeat_initial_delay;
    g_wldcore_input_repeat_period = g_main_input_repeat_period;
    g_wldcore_input_secondary_repeat_period = g_main_input_secondary_repeat_period;
}
