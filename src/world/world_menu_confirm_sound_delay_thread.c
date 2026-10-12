#include "fft/world.h"

/* Mini-menu thread: play the confirm sound and hold the slot for 16 frames. */
void world_menu_confirm_sound_delay_thread(void) {
    g_world_sound_pending_sfx_id = MAIN_SFX_CONFIRM;
    world_thread_set_current_task_id(NATIVE_THREAD_TASK_MINI_MENU);
    world_thread_wait_frames(0x10);
    world_thread_exit_current();
}
