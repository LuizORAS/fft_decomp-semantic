#include "fft/script.h"

/* Return how many frames the buttons have been held unchanged, the count
 * battle_script_update_event_input_state compares with the initial repeat delay. The scrolling ability
 * list scrolls on every other frame at event speed 1 by it, and repeats Up and Down once the delay has
 * passed. */
u32 battle_script_get_event_initial_repeat_counter(void) {
    return g_script_event_initial_repeat_counter;
}
