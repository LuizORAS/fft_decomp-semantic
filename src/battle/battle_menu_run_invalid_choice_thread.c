#include "fft/battle_menu.h"
#include "fft/sound.h"
#include "fft/thread.h"

/* Thread entry in g_battle_menu_idle_action_entries: queue the invalid-choice
 * sound and end the thread. */
void battle_menu_run_invalid_choice_thread(void) {
    sound_queue_sfx_invalid();
    thread_exit_current();
}
