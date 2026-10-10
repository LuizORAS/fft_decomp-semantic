#include "fft/battle.h"
#include "psx/types.h"

/* Thread entry of the mini menu (the Triangle menu): wait for the camera and any
 * menu transition to finish, pulse tutorial wait value 0xfe, show numeric values
 * again and run the icon selection loop. */
void battle_menu_display_triangle_selection(void) {
    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_MINI_MENU);
    while (battle_camera_is_active() != 0 || g_battle_menu_transition_state != 0) {
        battle_thread_yield();
    }
    battle_script_pulse_tutorial_wait_value(0xFE);
    g_battle_menu_hide_numeric_values = 0;
    battle_menu_run_icon_selection_loop();
}
