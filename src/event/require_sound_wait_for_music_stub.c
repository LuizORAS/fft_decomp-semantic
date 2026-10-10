#include "fft/event_require.h"
#include "psx/types.h"

/* Wait, a frame at a time, for the music load poll on the main stack to
 * report idle. The poll is main_return_zero_80043708, so the wait is always
 * a single VSync. */
void require_sound_wait_for_music_stub(void) {
    do {
        VSync(0);
        g_battle_thread_call_target = (void (*)(void))main_sound_is_music_loading_stub;
    } while (battle_thread_call_on_main_stack() != 0);
}
