#include "fft/script.h"
#include "fft/thread.h"

/* Thread body of EffectEnd (task THREAD_TASK_PLAY_EFFECT): play the effect that Effect and
 * EffectStart prepared and exit when it has finished. BATTLE and WORLD compile this text. */
void script_play_effect_thread(void) {
    thread_set_current_task_id(THREAD_TASK_PLAY_EFFECT);
    script_play_effect_and_wait();
    thread_exit_current();
}
