#include "fft/world.h"
#include "psx/types.h"

void world_script_reset_event_state(void) {
    world_unit_reset_animation_states();
    if (world_thread_is_running_asm(NATIVE_THREAD_SLOT_EVENT) == 0) {
        world_camera_store_state_to_script_variables();
        world_map_reset_script_light_transform();
        g_world_camera_speed_curve = 0;
        g_world_camera_position_locked = 0;
    }
}
