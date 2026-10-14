#include "fft/script.h"
#include "fft/thread.h"
#include "psx/pad.h"

/* Pause: yield until Select, Cross or Circle is pressed, or until the thread's
 * task id becomes the stop request. */
void script_pause_event_instruction(void) {
    do {
        thread_yield();
        if (thread_get_current_task_id() == THREAD_TASK_STOP_REQUEST) {
            return;
        }
    } while ((g_script_event_input & (PSX_PAD_SELECT | PSX_PAD_CROSS | PSX_PAD_CIRCLE)) == 0);
}
