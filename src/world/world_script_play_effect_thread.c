#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"

void world_script_play_effect_thread(void) {
    thread_set_current_task_id(NATIVE_THREAD_TASK_PLAY_EFFECT);
    script_play_effect_and_wait();
    thread_exit_current();
}
