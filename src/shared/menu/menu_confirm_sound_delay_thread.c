#include "fft/main.h"
#include "fft/menu.h"
#include "fft/sound.h"
#include "fft/thread.h"

/* Thread body: request the confirm sound and hold task id MINI_MENU for 16
 * frames. Nothing in the game starts it. WORLD compiles this text too. */
void menu_confirm_sound_delay_thread(void) {
    g_sound_pending_sfx_id = MAIN_SFX_CONFIRM;
    thread_set_current_task_id(THREAD_TASK_MINI_MENU);
    thread_wait_frames(16);
    thread_exit_current();
}
