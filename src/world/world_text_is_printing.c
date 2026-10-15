#include "fft/world.h"

/* Return whether the dialog thread in slot `thread_id` has task RESUME, which it sets as it starts and
 * after a wait; it has another task while it waits at 0xFD (WAIT_FOR_RESUME) or for its next text after 0xFE
 * (DIALOG_AWAIT_TEXT). */
s32 world_text_is_printing(s32 thread_id) {
    native_thread_t* thread = &g_world_threads[thread_id];

    return thread->task_id == NATIVE_THREAD_TASK_RESUME;
}
