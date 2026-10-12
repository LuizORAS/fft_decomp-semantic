#include "fft/battle.h"

/* Thread body: request the confirm sound and hold task id MINI_MENU for 16
 * frames. Nothing in the game starts it. */
void battle_menu_confirm_sound_delay_thread(void) {
    g_sound_pending_sfx_id = MAIN_SFX_CONFIRM;
    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_MINI_MENU);
    battle_thread_wait_frames(0x10);
    battle_thread_exit_current();
}
