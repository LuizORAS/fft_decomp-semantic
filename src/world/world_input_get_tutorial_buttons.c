#include "fft/world.h"
#include "psx/types.h"

/* Return the tutorial's scripted button word (g_world_tutorial_buttons), for the event input update
 * (world_script_update_event_input_state). */
s32 world_input_get_tutorial_buttons(void) {
    return g_world_tutorial_buttons;
}
