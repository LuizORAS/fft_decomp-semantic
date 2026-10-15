#include "fft/world.h"

/* Clear WORLD's menu input: the newly pressed buttons and both repeat words. */
void world_input_clear_state(void) {
    g_world_input_newly_pressed = 0;
    g_world_input_secondary_repeat = 0;
    g_world_input_primary_repeat = 0;
}
