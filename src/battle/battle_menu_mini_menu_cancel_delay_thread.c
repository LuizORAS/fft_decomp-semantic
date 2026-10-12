#include "fft/battle.h"

/* Thread entry of system functions 4, 5, 8 (close) and 0x0e: after two frames
 * post command 7 as the frame's menu selection and exit. The table lists it for
 * 0x1f as well, which battle_menu_run_system_function_thread overrides.
 *
 * Its MINI_MENU task id keeps the menu frame from forcing the event speed while
 * it runs. */
void battle_menu_mini_menu_cancel_delay_thread(void) {
    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_MINI_MENU);
    battle_thread_wait_frames(2);
    g_battle_menu_pending_selection[0] = 7;
    battle_thread_exit_current();
}
