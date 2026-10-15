#include "fft/world.h"
#include "psx/types.h"

/* Thread body of EffectEnd (task NATIVE_THREAD_TASK_PLAY_EFFECT): play the staged effect and exit
 * when it has finished. */
void world_script_play_effect_thread(void) {
    world_thread_set_current_task_id(NATIVE_THREAD_TASK_PLAY_EFFECT);
    world_script_play_effect_and_wait();
    world_thread_exit_current();
}
