#include "fft/world.h"
#include "psx/types.h"

void world_gfx_ramp_screen_overlay_intensity_to_zero(void) {
    g_world_script_tutorial_thread_switch_counter = 0xff;
    do {
        world_thread_yield();
        g_world_script_tutorial_thread_switch_counter -= 4;
    } while (g_world_script_tutorial_thread_switch_counter > 0);
    g_world_script_tutorial_thread_switch_counter = 0;
}
