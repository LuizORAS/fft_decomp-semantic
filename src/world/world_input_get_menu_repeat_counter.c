#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/* Return the menu input's initial repeat counter (g_script_event_initial_repeat_counter), which
 * the scrolling lists read. */
s32 world_input_get_menu_repeat_counter(void) {
    return g_script_event_initial_repeat_counter;
}
