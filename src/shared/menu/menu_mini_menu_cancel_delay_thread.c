#include "fft/menu.h"
#include "fft/thread.h"

/* Thread entry of system functions 4, 5, 8 (close) and 0x0e: after two frames
 * post command 7 as the frame's menu selection and exit. The table lists it for
 * 0x1f as well, which battle_menu_run_system_function_thread overrides. WORLD compiles this text too.
 *
 * Its MINI_MENU task id keeps the menu frame from forcing the event speed while
 * it runs. */
void menu_mini_menu_cancel_delay_thread(void) {
    thread_set_current_task_id(THREAD_TASK_MINI_MENU);
    thread_wait_frames(2);
    g_menu_pending_selection[0] = 7;
    thread_exit_current();
}
