#include "fft/wldcore.h"
#include "psx/libetc.h"
#include "psx/libgpu.h"

/* End a world map visit: wait for the GPU, VSync and the audio queue with the display off, unload
 * the scenario music (kept for a tutorial event), and publish the next map (g_wldcore_next_map_id)
 * as EVENT_SCRIPT_VAR_CURRENT_MAP. */
void wldcore_system_finalize_loop(void) {
    DrawSync(0);
    VSync(0);
    SetDispMask(0);
    wldcore_sound_wait_for_queue_drain();
    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_TUTORIAL_EVENT) == 0) {
        main_sound_unload_scenario_music_and_stop_tune();
    }
    world_script_set_variable(EVENT_SCRIPT_VAR_CURRENT_MAP, g_wldcore_next_map_id[0]);
}
