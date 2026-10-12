#include "fft/battle_gfx.h"
#include "fft/world.h"

/* Resets unit animation states and, when the current-event script variable is set, clears the screen
 * colour modulation and starts the next event on thread 1. */
void world_script_start_current_event_thread(void) {
    world_unit_reset_animation_states();
    g_world_script_attack_entry_mode = SCRIPT_ATTACK_ENTRY_NORMAL;
    if (world_script_get_variable(EVENT_SCRIPT_VAR_BATTLE_NOT_STARTED) != 0) {
        world_camera_store_state_to_script_variables();
        world_map_reset_script_light_transform();
        g_world_camera_position_locked = 0;
        g_world_camera_speed_curve = 0;
    }
    if (world_script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT) != 0) {
        g_world_camera_position_locked = 0;
        g_world_camera_speed_curve = 0;
        battle_gfx_start_screen_color_modulation_fade(2, 0, 0, 0, 2);
        world_thread_start(NATIVE_THREAD_SLOT_EVENT, world_script_run_next_event);
    }
}
