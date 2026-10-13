#include "fft/battle_camera.h"
#include "fft/battle_map.h"
#include "fft/battle_script.h"
#include "fft/battle_unit.h"
#include "fft/thread.h"

/* Battle twin of world_script_reset_event_state. Where WORLD has no-op stubs, BATTLE
 * snapshots the camera into script variables and resets the script camera
 * transform while no thread occupies slot 1. */
void script_reset_event_state(void) {
    unit_reset_animation_states();
    if (battle_thread_is_running_asm(NATIVE_THREAD_SLOT_EVENT) == 0) {
        camera_store_state_to_script_variables();
        map_reset_script_light_transform();
        g_camera_speed_curve = 0;
        g_camera_position_locked = 0;
    }
}
