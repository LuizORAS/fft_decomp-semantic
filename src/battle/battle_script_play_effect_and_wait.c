#include "fft/battle.h"
#include "psx/types.h"

/* Plays the current effect through the inner-subroutine caller, then yields
 * to other threads until the animation continue check reports done. */
void battle_script_play_effect_and_wait(void) {
    g_battle_thread_call_target = battle_effect_call_play;
    battle_thread_call_on_main_stack();

    do {
        battle_thread_yield();
        g_battle_thread_call_target = (void (*)(void))battle_state_get_animation_continue_check;
    } while (battle_thread_call_on_main_stack());
}
