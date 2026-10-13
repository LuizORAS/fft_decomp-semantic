#include "fft/main.h"
#include "fft/menu.h"
#include "fft/sound.h"
#include "fft/thread.h"

/* Queue the confirm sound and hold the mini-menu task id for 16 frames. */
void menu_confirm_sound_delay_thread(void) {
    g_sound_pending_sfx_id = MAIN_SFX_CONFIRM;
    thread_set_current_task_id(NATIVE_THREAD_TASK_MINI_MENU);
    thread_wait_frames(16);
    thread_exit_current();
}
