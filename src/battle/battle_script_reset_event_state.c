#include "fft/battle.h"
#include "psx/types.h"

/* Reset the unit animation states and, while no event thread runs (NATIVE_THREAD_SLOT_EVENT), store
 * the camera in the script variables, put the script light transform back to the map's light and
 * clear the camera speed curve and position lock. battle_state_enter_event calls it; in WORLD's copy
 * the camera and light calls are empty functions. */
void battle_script_reset_event_state(void) {
    battle_unit_reset_animation_states();
    if (battle_thread_is_running_asm(NATIVE_THREAD_SLOT_EVENT) == 0) {
        battle_camera_store_state_to_script_variables();
        battle_map_reset_script_light_transform();
        g_battle_camera_speed_curve = 0;
        g_battle_camera_position_locked = 0;
    }
}
