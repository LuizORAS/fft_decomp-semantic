#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 45: stop the background (weather) sound and forget the recorded one. */
void wldcore_sound_novel_stop_background_sound(void) {
    main_sound_stop_weather_sfx_music();
    g_wldcore_sound_novel_record.sound_id = 0;
    g_wldcore_sound_novel_state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
