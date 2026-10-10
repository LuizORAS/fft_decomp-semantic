#include "fft/world.h"
#include "psx/types.h"

void world_sound_wait_for_music_stub(void) {
    do {
        VSync(0);
        g_world_thread_call_target = (void (*)(void))main_sound_is_music_loading_stub;
    } while (world_thread_call_on_main_stack() != 0);
}
