#include "fft/world.h"
#include "psx/types.h"

/* Keep a copy of the raw 32-bit controller value, then apply the script's input
 * overrides to it in place. */
void world_script_filter_controller_input(u32* src) {
    g_world_input_unfiltered_controller = *src;
    world_script_apply_input_overrides(src);
}
