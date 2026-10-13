#include "fft/main.h"
#include "fft/sound.h"
#include "fft/thread.h"
#include "psx/libetc.h"

/* Wait one vertical blank before polling the music-loading stub, which
 * always reports idle. */
void sound_wait_for_music_stub(void) {
    do {
        VSync(0);
        g_thread_call_target = main_sound_is_music_loading_stub;
    } while (thread_call_on_main_stack() != 0);
}
