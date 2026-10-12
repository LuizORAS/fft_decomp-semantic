#include "fft/world.h"
#include "psx/types.h"

/* Return the input lockout timer (world_input_update_lockout_timer). While it is nonzero
 * world_input_filter_menu passes no input, and the formation grid and the fitting room hold some
 * buttons back. */
s32 world_input_get_lockout_timer(void) {
    return g_world_input_lockout_timer;
}
