#include "fft/battle_camera.h"
#include "fft/thread.h"
#include "fft/world.h"

/* Menu thread of the Triangle mini menu: waits out any camera move and menu
 * transition, pulses tutorial wait value 0xFE and runs the icon selection
 * loop with the numeric values shown. */
void world_menu_display_triangle_selection(void) {
    world_thread_set_current_task_id(NATIVE_THREAD_TASK_MINI_MENU);
    while (camera_is_active() != 0 || g_world_menu_transition_timeout != 0) {
        thread_yield();
    }
    world_script_pulse_tutorial_wait_value(0xFE);
    g_world_menu_hide_numeric_values = 0;
    world_menu_run_icon_selection_loop();
}
