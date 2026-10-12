#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_event.h"
#include "fft/battle_gfx.h"
#include "fft/battle_map.h"
#include "fft/battle_unit.h"
#include "fft/thread.h"

/* Reset the unit animation states and, when the current-event variable is set,
 * fade the screen colour modulation to zero and start the event thread (slot 1)
 * on battle_script_run_next_event.
 *
 * While the battle has not started it first stores the camera in the script
 * variables and resets the script camera transform. Where WORLD's copy calls two
 * empty functions, BATTLE does that camera work. */
void battle_script_start_current_event_thread(void) {
    battle_unit_reset_animation_states();
    g_battle_script_attack_entry_mode = SCRIPT_ATTACK_ENTRY_NORMAL;
    if (battle_script_get_variable(EVENT_SCRIPT_VAR_BATTLE_NOT_STARTED) != 0) {
        battle_camera_store_state_to_script_variables();
        battle_map_reset_script_light_transform();
        g_battle_camera_position_locked = 0;
        g_battle_camera_speed_curve = 0;
    }
    if (battle_script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT) != 0) {
        g_battle_camera_position_locked = 0;
        g_battle_camera_speed_curve = 0;
        battle_gfx_start_screen_color_modulation_fade(2, 0, 0, 0, 2);
        battle_thread_start(NATIVE_THREAD_SLOT_EVENT, battle_script_run_next_event);
    }
}
