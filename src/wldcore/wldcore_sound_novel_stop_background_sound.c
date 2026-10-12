#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 45: stop the background (weather) sound and forget the recorded one. */
void wldcore_sound_novel_stop_background_sound(void) {
    u16 flags;

    main_sound_stop_weather_sfx_music();
    flags = g_wldcore_sound_novel_state_flags;
    g_wldcore_sound_novel_record.sound_id = 0;
    g_wldcore_sound_novel_state_flags = flags | WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
