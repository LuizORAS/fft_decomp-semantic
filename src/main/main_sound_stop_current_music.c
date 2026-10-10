#include "fft/main.h"
#include "psx/types.h"

/* Keys off the playing scenario music and clears the current-music state;
 * its slot stays loaded. */
s32 main_sound_stop_current_music(void) {
    suzuki_music_t** music = &g_main_sound_music.state.current_music;
    s32 result = 0;

    if (*music != 0) {
        SuzukiDeallocateMUSChannels(*music);
        result = 1;
        g_main_sound_music.state.current_slot = 0;
        g_main_sound_music.state.current_smd = 0;
        *music = 0;
    }
    return result;
}
