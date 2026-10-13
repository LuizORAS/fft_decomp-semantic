#include "fft/thread.h"
#include "fft/world.h"

/* Release a dialog thread that is waiting for text: its task becomes RESUME
 * and its second parameter 0xffff. */
void world_text_resume_printing(s32 thread_id) {
    native_thread_t* thread = &g_native_threads[thread_id];

    if (thread->task_id == NATIVE_THREAD_TASK_DIALOG_AWAIT_TEXT) {
        thread->task_id = NATIVE_THREAD_TASK_RESUME;
        thread->function_parameter_2 = 0xffff;
    }
}
