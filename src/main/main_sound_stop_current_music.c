#include "fft/main.h"
#include "psx/types.h"

/* Keys off the playing scenario music and clears the current-music state;
 * its slot stays loaded. */
s32 main_sound_stop_current_music(void) {
    suzuki_music_t** music = &g_main_sound_music.state.current_music;

    if (*music != 0) {
        SuzukiDeallocateMUSChannels(*music);
        g_main_sound_music.state.current_slot = 0;
        g_main_sound_music.state.current_smd = 0;
        *music = 0;
        return 1;
    }
    return 0;
}
