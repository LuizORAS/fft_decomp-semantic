#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound-novel opcode: stop the weather sound and clear the recorded sound. */
void wldcore_opcode_stop_music(void) {
    main_sound_stop_weather_sfx_music();
    g_wldcore_active_saved_record.sound_id = 0;
    g_wldcore_state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
