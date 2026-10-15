#include "fft/battle.h"
#include "psx/types.h"

/* Thread body of EffectEnd: play the effect that Effect and EffectStart prepared
 * and exit when it has finished. */
void battle_script_play_effect_thread(void) {
    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_PLAY_EFFECT);
    battle_script_play_effect_and_wait();
    battle_thread_exit_current();
}
