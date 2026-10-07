#include "fft/world.h"

/* Start function in slot thread_id: its stack begins at the slot's top, the slot is marked
 * running and its task id, parameter 4 and task words are cleared. It first runs at the next
 * scheduler pass. */
void world_thread_start(s32 thread_id, void (*function)(void)) {
    void* global_pointer = world_thread_get_current_global_pointer();
    native_thread_t* thread = &g_world_threads[thread_id];

    thread->global_pointer = global_pointer;
    thread->stack_pointer = thread->stack_top;
    thread->frame_pointer = thread->stack_top;
    thread->code_pointer = function;
    thread->is_running = 1;
    thread->task_id = 0;
    thread->function_parameter_4 = 0;
    thread->task_words[0] = 0;
    thread->task_words[1] = 0;
    thread->task_words[2] = 0;
    thread->task_words[3] = 0;
    thread->task_words[4] = 0;
    thread->task_words[5] = 0;
    thread->task_words[6] = 0;
}
