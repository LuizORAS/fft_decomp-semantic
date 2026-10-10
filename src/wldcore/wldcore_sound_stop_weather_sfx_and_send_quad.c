#include "fft/wldcore.h"
#include "psx/types.h"

enum {
    WLDCORE_SOUND_QUAD_COMMAND_COUNT = 4,
    WLDCORE_SOUND_QUAD_COMMAND_BASE = 0xE10000,
};

/* Stop weather audio and play the selected row of four sounds.
 *
 * Each table byte is a sound number in the bank with id 0xE1 that WLDCORE.BIN
 * carries; main_sound_play_4_sfx starts the four on SFX voice pairs 0, 2, 4
 * and 6. A zero byte selects that bank's empty sound 0, which stops the
 * pair. */
void wldcore_sound_stop_weather_sfx_and_send_quad(s32 sound_id) {
    s32 values[WLDCORE_SOUND_QUAD_COMMAND_COUNT];
    s32 i;

    main_sound_stop_weather_sfx_music();
    for (i = 0; i < WLDCORE_SOUND_QUAD_COMMAND_COUNT; i++) {
        values[i] = g_wldcore_sound_quad_command_bytes[sound_id][i] + WLDCORE_SOUND_QUAD_COMMAND_BASE;
    }
    main_sound_play_4_sfx(values[0], values[1], values[2], values[3]);
}
