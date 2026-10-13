#include "fft/battle.h"
#include "fft/battle_effect.h"
#include "fft/battle_script.h"
#include "fft/thread.h"

/* Plays the current effect through the inner-subroutine caller, then yields
 * to other threads until the animation continue check reports done. */
void script_play_effect_and_wait(void) {
    g_thread_call_target = effect_call_play;
    thread_call_on_main_stack();

    do {
        thread_yield();
        g_thread_call_target = (void (*)(void))battle_state_get_animation_continue_check;
    } while (thread_call_on_main_stack());
}
