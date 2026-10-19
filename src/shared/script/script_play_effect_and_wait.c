#include "fft/battle.h"
#include "fft/battle_effect.h"
#include "fft/script.h"
#include "fft/thread.h"

/* Play the staged effect on the main stack (battle_effect_call_play), then yield until the animation
 * continue check reports it done. UnitAnim calls it after a bow animation. */
void script_play_effect_and_wait(void) {
    g_thread_call_target = battle_effect_call_play;
    thread_call_on_main_stack();

    do {
        thread_yield();
        g_thread_call_target = (void (*)(void))battle_state_get_animation_continue_check;
    } while (thread_call_on_main_stack());
}
