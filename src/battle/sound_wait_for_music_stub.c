#include "fft/battle_sound.h"
#include "fft/battle_thread.h"

/* Wait, one vertical blank at a time, while main_return_zero_80043708 called on
 * the main stack returns nonzero. That function always returns 0, so the wait
 * lasts exactly one vertical blank. */
void sound_wait_for_music_stub(void) {
    do {
        VSync(0);
        g_battle_thread_call_target = main_sound_is_music_loading_stub;
    } while (battle_thread_call_on_main_stack() != 0);
}
