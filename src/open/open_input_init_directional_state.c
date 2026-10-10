#include "fft/open.h"
#include "psx/types.h"

/* Clear the direction repeat counters and take the repeat delay and period from
 * MAIN's input timings. */
void open_input_init_directional_state(void) {
    s32 first_limit = g_main_input_repeat_initial_delay;
    s32 second_limit = g_main_input_repeat_period;

    g_open_input_right_repeat_counter = 0;
    g_open_input_left_repeat_counter = 0;
    g_open_input_down_repeat_counter = 0;
    g_open_input_up_repeat_counter = 0;
    g_open_input_directional_repeat_initial_delay = first_limit;
    g_open_input_directional_repeat_period = second_limit;
}
