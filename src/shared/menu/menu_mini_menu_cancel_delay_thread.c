#include "fft/menu.h"
#include "fft/thread.h"

/* Post the close command after two frames under the mini-menu task id. */
void menu_mini_menu_cancel_delay_thread(void) {
    thread_set_current_task_id(NATIVE_THREAD_TASK_MINI_MENU);
    thread_wait_frames(2);
    g_menu_pending_selection[0] = 7;
    thread_exit_current();
}
