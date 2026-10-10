#include "fft/world.h"
#include "psx/types.h"

void world_script_filter_controller_input(u32* src) {
    g_world_input_unfiltered_controller = *src;
    world_script_apply_input_overrides(src);
}
