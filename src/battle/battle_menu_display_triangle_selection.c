#include "fft/battle_camera.h"
#include "fft/battle_menu.h"
#include "fft/battle_script.h"
#include "fft/menu.h"
#include "fft/thread.h"

/* Thread entry of the mini menu (the Triangle menu): wait for the camera and any
 * menu transition to finish, pulse tutorial wait value 0xfe, show numeric values
 * again and run the icon selection loop. */
void battle_menu_display_triangle_selection(void) {
    thread_set_current_task_id(THREAD_TASK_MINI_MENU);
    while (camera_is_active() != 0 || g_battle_menu_transition_state != 0) {
        thread_yield();
    }
    script_pulse_tutorial_wait_value(0xFE);
    g_menu_hide_numeric_values = 0;
    battle_menu_run_icon_selection_loop();
}
