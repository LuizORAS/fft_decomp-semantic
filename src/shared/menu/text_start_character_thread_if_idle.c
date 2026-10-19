#include "fft/battle_text.h"
#include "fft/menu.h"
#include "fft/thread.h"

/* Start the dialog thread (world_text_character_handling_thread) in slot `thread_id` with parameters 1-3,
 * unless that slot is already running a thread. */
void text_start_character_thread_if_idle(s32 thread_id, s32 parameter_0, s32 parameter_1, s32 parameter_2) {
    if (thread_is_running(thread_id) == 0) {
        thread_start(thread_id, battle_text_run_dialog_thread);
        thread_set_parameters(thread_id, parameter_0, parameter_1, parameter_2);
    }
}
