#include "fft/battle_script.h"
#include "fft/battle_thread.h"
#include "fft/script.h"
#include "fft/thread.h"

/* Thread body of EffectEnd: play the effect that Effect and EffectStart prepared
 * and exit when it has finished. */
void script_play_effect_thread(void) {
    thread_set_current_task_id(NATIVE_THREAD_TASK_PLAY_EFFECT);
    script_play_effect_and_wait();
    thread_exit_current();
}
