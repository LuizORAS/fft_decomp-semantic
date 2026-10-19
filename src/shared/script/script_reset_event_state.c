#include "fft/battle_camera.h"
#include "fft/battle_map.h"
#include "fft/battle_unit.h"
#include "fft/script.h"
#include "fft/thread.h"

/* Reset the unit animation states and, while no event thread runs (THREAD_SLOT_EVENT), store
 * the camera in the script variables, put the script light transform back to the map's light and
 * clear the camera speed curve and position lock. battle_state_enter_event calls it; in WORLD's copy
 * the camera and light calls are empty functions. */
void script_reset_event_state(void) {
    unit_reset_animation_states();
    if (battle_thread_is_running_asm(THREAD_SLOT_EVENT) == 0) {
        camera_store_state_to_script_variables();
        map_reset_script_light_transform();
        g_camera_speed_curve = 0;
        g_camera_position_locked = 0;
    }
}
