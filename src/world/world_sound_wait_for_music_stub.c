#include "fft/thread.h"
#include "fft/world.h"

/* Wait, a frame at a time, for the music load poll on the main stack to
 * report idle. The poll is main_return_zero_80043708, so the wait is always
 * a single VSync. */
void world_sound_wait_for_music_stub(void) {
    do {
        VSync(0);
        g_thread_call_target = (void (*)(void))main_sound_is_music_loading_stub;
    } while (thread_call_on_main_stack() != 0);
}
